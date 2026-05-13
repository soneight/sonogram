#ifndef APP_HXX
#define APP_HXX

#include <app/alias.hxx>
#include <app/limits.hxx>
#include <app/locale.hxx>
#include <app/token.hxx>
#include <app/util.hxx>
// son8
// -- c
#include <son8/c/byte.hxx>
// -- cxx
#include <son8/cxx/data.hxx>
#include <son8/cxx/file.hxx>

namespace app {

   // data aliases (move to core_lib?)
   template< typename Type >
   using Dual = cxx::list< Type >;
   template< typename Type >
   using Grow = cxx::vector< Type >;
   template< typename Key, typename Value >
   using Hash = cxx::unordered_map< Key, Value >;
   template< typename Key, typename Value >
   using Keys = cxx::map< Key, Value >;
   template< typename Type >
   using Tail = cxx::forward_list< Type >;
   template< typename Type >
   using Tidy = cxx::set< Type >;
   // TODO: this is temporary solution for global source string handling
   class Source final {
      bool assigned_{ };
      String data_;
   public:
      // default
      Source( ) = default;
     ~Source( ) = default;
      // delete
      Source( Uni< Source > move ) = delete;
      Source( Ref< Source > copy ) = delete;
      APP_PROC operator=( Ref< Source > copy ) = delete;
      // impl
      Source( Uni< String > str ) noexcept : data_{ cxx::move( str ) } { }
      APP_PROC operator=( Uni< Source > move ) {
         if ( assigned_ ) throw cxx::runtime_error{ "Source could be assigned only once" };
         data_ = cxx::move( move.data_ );
         assigned_ = true;
      }
      // accessors
      APP_FUNC get( ) const -> Ref< String > { return data_; }
   };

   using Tokens = Grow< Token >;

   APP_FUNC lex_tokens( Ref< String > str ) -> Tokens {
      // NOTE: not an error as initial file reading should catch this
      APP_ASSERT( str.front( ) != '\n' and str.back( ) == '\n' and "source should end with new line or begin with it" );
      Tokens tokens;
      Size const size{ str.size( ) };
      tokens.reserve( size >> 3 );
      tokens.emplace_back( Token::View{ "\0"sv }, 0, 0, Token::Kind::Apptype_Unknown );
      Size pos{ }, curLine{ 1u }, curColn{ };
      Char ch;
      auto next_char = [&]( ) -> bool {
         ++curColn;
         ch = APP_CAST( Char, str[pos++] );
         // NOTE: current scheme with no checking for false condition
         // \ allow to avoid such checks in other scan lambdas
         // \ this is possible because source required to end with new line
         // \ but be aware to not introduce errors when looking too ahead
         return true;
      };
      auto scan_identifier = [&]( ) {
         while ( next_char( ) and Locale::is_alnum( ch ) );
         return Token::Kind::Identifier;
      };
      auto scan_spaces = [&]( ) {
         while ( next_char( ) and Locale::is_blank( ch ) );
         return Token::Kind::Space;
      };
      auto scan_comment = [&]( ) {
         if ( next_char( ) and not Locale::is_blank( ch ) ) throw Error{ "app::lex_tokens: no space character after comment symbol" };
         while ( next_char( ) and not ( ch == '\n' ) );
         return Token::Kind::Comment;
      };
      auto scan_keyword = [&]( ) {
         auto prevPos = pos - 1;
         while ( next_char( ) and Locale::is_alpha( ch ) );
         auto kind = Token::Keywords::view_to_kind( str.substr( prevPos, pos - prevPos - 1 ) );
         bool isApp = ( kind == Token::Kind::Keyword_Program );
         if ( isApp and tokens[0].kind != Token::Kind::Apptype_Unknown ) throw Error{ "app::lex_tokens: application type duplicate" };
         else tokens[0] = Token{ "\0"sv, 0, 0, Token::Kind::Apptype_Program };
         return kind;
      };
      auto scan_singles = [&]( ) {
         Token::Kind kind;
         switch ( ch ) {
         case ':': kind = Token::Kind::Scope_Opened; break;
         case ';': kind = Token::Kind::Scope_Closed; break;
            default: throw Error{ "app::lex_tokens: unknown character to process" };
         }
         next_char( );
         return kind;
      };

      while ( next_char( ) ) {
         if ( ch == '\n' ) {
            Token::Ref prevToken = tokens.back( );
            if ( prevToken.kind == Token::Kind::Space ) throw Error{ "app::lex_tokens: trailing whitespace" };
            if ( pos >= size ) break; // end of source
            ++curLine;
            curColn = 0;
            continue;
         }
         auto prevPos = pos - 1;
         auto prevCol = curColn;
         auto kind = Token::Kind::Last_;
         if ( Locale::is_alpha( ch ) ) kind = scan_identifier( );
         else if ( Locale::is_blank( ch ) ) kind = scan_spaces( );
         else if ( ch == '#' ) kind = scan_comment( );
         else if ( ch == '-' ) kind = scan_keyword( );
         else kind = scan_singles( );
         // NOTE: some funcs may want to return Last_ kind to continue a process
         if ( kind == Token::Kind::Last_ ) continue;
         --pos;
         --curColn;
         auto view = [&str,pos,prevPos]( ) { return Token::View{ &str[prevPos], pos - prevPos }; };
         tokens.emplace_back( view( ), curLine, prevCol, kind );
      }
      tokens.emplace_back( "\0"sv, 0, 0, Token::Kind::Last_ );
      return tokens;
   }
   // program
   struct Program final {
      // states
      enum class State : Unt0 {
         Global,
         Body,
      };
      // data members
      State state{ };
      String fileName{ };
      String mainHead;
      String mainBody{ };
      String mainFoot;
      Program( )
      : fileName{ "program" }
      , mainHead{ "int main( ) {" }
      , mainFoot{ "}" } { }
   };
   APP_FUNC to_string( Token::Kind kind ) -> String;
   APP_FUNC to_string( Token const &token ) -> String;
   APP_FUNC gen_program( Ref< Tokens > tokens ) -> Program {
      using Kind = Token::Kind;
      using State = Program::State;
      constexpr int Scope_Opened = 0;
      constexpr int Scope_Closed = 1;
      Program program;
      // auto const itEnd = tokens.end( );
      auto itPos = tokens.begin( ) + 1;
      Kind expectKind = Kind::Identifier;
      Token token = tokens.back( );
      if ( token.kind != Kind::Last_ ) throw Error{ "app::gen_program: tokens does not ends with last terminator" };
      auto next_token = [&]( ) -> bool {
         token = *itPos++;
         return token.kind != Kind::Last_;
      };
      int scopeDepth{ };
      auto parse_scope = [&]( int scope ) {
         if ( program.state == State::Global and expectKind != Kind::Scope_Opened ) {
            throw Error{ "app::gen_program expect open score in global state" };
         }
         program.state = State::Body;
         int d[2] = { scopeDepth + 1, scopeDepth - 1 };
         scopeDepth = d[scope];
         if ( scopeDepth < 0 ) throw Error{ "app::gen_program: scope depth negative" };
      };
      auto parse_program = [&]( ) {
         if ( program.state == State::Global ) {
            if ( expectKind != Kind::Keyword_Program ) throw Error{ "app::gen_program: expect keyword program in global state" };
            expectKind = Kind::Scope_Opened;
         }
      };
      auto parse_identifier = [&]( ) {
         if ( program.state == State::Global ) {
            program.fileName = token.view;
            expectKind = Kind::Keyword_Program;
         }
      };

      while ( next_token( ) ) {
         switch ( token.kind ) {
         case Kind::Comment: continue;
         case Kind::Space: continue;
         case Kind::Identifier: parse_identifier( ); continue;
         case Kind::Keyword_Program: parse_program( ); continue;
         case Kind::Scope_Opened: parse_scope( Scope_Opened ); continue;
         case Kind::Scope_Closed: parse_scope( Scope_Closed ); continue;
         case Kind::Last_: break;
            default: throw Error{ "app::gen_program: token is not supported yet" + to_string( token ) };
         }
         switch ( program.state ) {
            default: continue;
         }
      }

      if ( scopeDepth ) throw Error{ "app::gen_program: scope depth not equal zero: " + cxx::to_string( scopeDepth) };

      return program;
   }

} // namespace app

#endif//APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
