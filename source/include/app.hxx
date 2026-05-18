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

   struct BufferBraces final {
      using Pairs = Flat< cxx::pair< Char, Char >, 4 >;
      APP_DATA pairs = Pairs{{
         { ':', ';' },
         { '{', '}' },
         { '(', ')' },
         { '[', ']' },
      }};
      APP_DATA Pair_Scope = 0u;
      APP_DATA Pair_Curly = 1u;
      APP_DATA Pair_Round = 2u;
      APP_DATA Pair_Array = 3u;
      APP_DATA Pair_Error = -1u;
      APP_DATA Data_Size = Max::Scoped_Depth + Max::Nested_Depth;
      Flat< Char, Data_Size > data{ };
      Unt0 scopedDepth{ };
      Unt0 nestedDepth{ };
      Unt0 idx{ };

      String datastr() const {
         String result;
         for ( auto i = 0u; i < idx; ++i ) result += data[i];
         return result;
      }
      bool is_exceed_nested_depth( ) { return nestedDepth > Max::Nested_Depth; }
      bool is_exceed_scoped_depth( ) { return scopedDepth > Max::Scoped_Depth; }
   };

   using Tokens = Grow< Token >;

   APP_FUNC lex_tokens( Ref< String > str ) -> Tokens {
      // NOTE: not an error as initial file reading should catch this
      APP_ASSERT( str.front( ) != '\n' and str.back( ) == '\n' and "source should end with new line or begin with it" );
      Tokens tokens;
      BufferBraces bufferBraces = { };
      APP_ASSERT( bufferBraces.idx == 0 );
      Size const size{ str.size( ) };
      tokens.reserve( size >> 3u );
      using Kind = Token::Kind;
      tokens.emplace_back( Token::View{ "\0"sv }, 0, 0, Kind::Apptype_Unknown );
      Size pos{ }, curLine{ 1u }, curColn{ };
      Char ch;
      auto next_char = [&]( ) -> bool {
         ++curColn;
         ch = APP_CAST( Char, str[pos++] );
         // NOTE: current scheme with no checking for false condition allow
         // \ to avoid such check in other scan characters lambda functions
         // \ this is possible because source required to end with new line
         // \ but been aware to not introduce errors when looking too ahead
         return true;
      };
      String errMsg;
      auto kind_error = [&errMsg]( app::Ref< String > message ) {
         errMsg = message;
         return Kind::Error;
      };
      auto scan_identifier = [&]( ) {
         Char prev = ch;
         while ( next_char( ) and ( Locale::is_alnum( ch ) || ch == '_' ) ) prev = ch;
         if ( prev == '_' ) return kind_error( "identifier cannot end with underscore character"s );
         return Kind::Identifier;
      };
      auto scan_spaces = [&]( ) {
         while ( next_char( ) and Locale::is_blank( ch ) );
         return Kind::Space;
      };
      auto scan_comment = [&]( ) {
         if ( next_char( ) and not Locale::is_blank( ch ) ) return kind_error( "no space character after comment symbol"s );
         while ( next_char( ) and not ( ch == '\n' ) );
         return Kind::Comment;
      };
      auto scan_keyword = [&]( ) {
         auto prevPos = pos - 1;
         while ( next_char( ) and Locale::is_alpha( ch ) );
         if ( Locale::is_digit( ch ) ) next_char( ); // NOTE: for int0, int1 and so on keyword types
         auto kind = Token::Keywords::view_to_kind( str.substr( prevPos, pos - prevPos - 1 ) );
         if ( kind == Kind::Error ) return kind_error( "unknown keyword"s );
         bool isAppAndUnknown = kind == Kind::Keyword_Program and tokens[0].kind != Kind::Apptype_Unknown;
         if ( isAppAndUnknown ) return kind_error( "application type duplicate"s );
         else tokens[0] = Token{ "\0"sv, 0, 0, Kind::Apptype_Program };
         return kind;
      };
      auto scan_literal = [&]( ) {
         auto checkSameEnd = ch;
         while ( next_char( ) and not ( ch == checkSameEnd ) ) {
            if ( ch == '\n' ) return kind_error( "new line character before closing quote literal"s );
         }
         next_char( ); // NOTE: capture last char as processing by default moving to prev char later
         switch ( checkSameEnd ) {
            case '\'': return Kind::Literal_Single;
            case '"' : return Kind::Literal_Double;
            case '`' : return Kind::Literal_Grave;
            default  : return kind_error( "scan_literal incorrect check same end"s );
         }
      };
      auto scan_number = [&]( ) {
         while ( next_char( ) and ( Locale::is_digit( ch ) or ch == '.' ) );
         return Kind::Number;
      };
      auto scan_binary = [&]( ) {
         auto prev = ch;
         if ( next_char( ) and not Locale::is_binary( ch ) ) return kind_error( "expect binary character"s );
         auto curr = ch;
         next_char( );
         if ( prev == '<' and curr == '=' ) return Kind::Less_Equal;
         if ( prev == '<' and curr == '<' ) return Kind::Less_Less;
         if ( prev == '=' and curr == '=' ) return Kind::Equal_Equal;
         if ( prev == '>' and curr == '>' ) return Kind::More_More;
         if ( prev == '<' and curr == '>' ) return Kind::Not_Equal;
         if ( prev == '=' and curr == '*' ) return Kind::Math_Mulptiply;
         if ( prev == '=' and curr == '/' ) return Kind::Math_Divide;
         if ( prev == '=' and curr == '+' ) return Kind::Math_Plus;
         if ( prev == '=' and curr == '-' ) return Kind::Math_Minus;
         return kind_error( "unknown binary operation"s );
      };
      auto scan_singles = [&]( ) {
         auto &idxBrace = bufferBraces.idx;
         auto &buffer = bufferBraces.data;
         auto pair = BufferBraces::Pair_Error;
         // NOTE: assert specifically catch if it significantly overflow
         // \ as overflow by only 1 are catch by Error exception handler
         APP_ASSERT( bufferBraces.idx <= BufferBraces::Data_Size );
         Kind kind;
         switch ( ch ) {
         case ':': kind = Kind::Scope_Opened; pair = BufferBraces::Pair_Scope; break;
         case ';': kind = Kind::Scope_Closed; pair = BufferBraces::Pair_Scope; break;
         case '{': kind = Kind::Curly_Opened; pair = BufferBraces::Pair_Curly; break;
         case '}': kind = Kind::Curly_Closed; pair = BufferBraces::Pair_Curly; break;
         case '(': kind = Kind::Round_Opened; pair = BufferBraces::Pair_Round; break;
         case ')': kind = Kind::Round_Closed; pair = BufferBraces::Pair_Round; break;
         case '[': kind = Kind::Array_Opened; pair = BufferBraces::Pair_Array; break;
         case ']': kind = Kind::Array_Closed; pair = BufferBraces::Pair_Array; break;
         case ',': { next_char( ); return Kind::Single_Comma; }
            default: {
               auto charStr = ( Locale::is_print( ch ) ) ? String{ APP_CAST( char, ch ) } : cxx::to_string( ch );
               return kind_error( "unknown character to process '"s + charStr + "'"s );
            }
         }
         // TODO: override to more sane version
         // NOTE: : and ; still need to be processed into a buffer
         // \ to check that it does not appear inside other braces
         if ( Token::is_bracket_opened( kind ) ) {
            if ( idxBrace == BufferBraces::Data_Size ) return kind_error( "braces overflow"s );
            // NOTE: an `:` cannot appear inside any of other bracket so
            // \ technically it can only be stacked up with itself thats
            // \ allow to check last buffer entrance and not full buffer
            if ( idxBrace and ch == ':' and buffer[idxBrace - 1] != ':' ) return kind_error( "opening scope ':' can only be nested with itself" );
            buffer[idxBrace++] = ch;
            if ( ch == ':' ) {
               ++bufferBraces.scopedDepth;
               if ( bufferBraces.is_exceed_scoped_depth( ) ) return kind_error( "scoped depth exceed maximum limit" );
            } else {
               ++bufferBraces.nestedDepth;
               if ( bufferBraces.is_exceed_nested_depth( ) ) return kind_error( "nested depth exceed maximum limit" );
            }
         } else { // not opened, closed
            if ( idxBrace == 0 ) return kind_error( "braces underflow"s  );
            Char curBrace = buffer[--idxBrace];
            if ( curBrace != BufferBraces::pairs[pair].first ) return kind_error( "braces mismatch"s );
            if ( ch == ';' ) --bufferBraces.scopedDepth;
            else --bufferBraces.nestedDepth;
         }
         next_char( );
         return kind;
      };
      // NOTE: scan functions use next_char to simplify their logic in a way
      // \ that the position (pos) is always move back one character after a
      // \ scan finishes, this "backtrack" preserve an invalid character for
      // \ the next token, but if a scan ends on a valid character then call
      // \ next_char an additional time to ensure that character is consumed
      // \ REASON OF INFINITE LOOP could be caused because of this behavior!
      while ( next_char( ) ) {
         if ( ch == '\n' ) {
            Token::Ref prevToken = tokens.back( );
            if ( prevToken.kind == Kind::Space ) {
               throw Error{ "app::lex_tokens: trailing whitespace at "s + to_string( tokens.back( ) ) };
            }
            if ( pos >= size ) break; // end of source
            ++curLine;
            curColn = 0;
            continue;
         }
         auto prevPos = pos - 1;
         auto prevCol = curColn;
         auto kind = Kind::Last_;
         if/*_*/ ( Locale::is_alpha( ch ) ) kind = scan_identifier( );
         else if ( Locale::is_blank( ch ) ) kind = scan_spaces( );
         else if ( ch == '#' ) kind = scan_comment( );
         else if ( ch == '-' ) kind = scan_keyword( );
         else if ( ch == '\'' or ch == '"' or ch == '`' ) kind = scan_literal( );
         else if ( ch == '+' ) kind = Kind::Unary_Plus, next_char( );
         else if ( Locale::is_digit( ch ) ) kind = scan_number( );
         else if ( Locale::is_binary(ch ) ) kind = scan_binary( );
         else kind = scan_singles( );
         if ( kind == Kind::Error ) throw Error{ "T?: "s + errMsg + " near "s + to_string( tokens.back( ) ) };
         // NOTE: some funcs may want to return Last_ kind to continue a process
         if ( kind == Kind::Last_ ) continue;
         --pos;
         --curColn;
         auto view = [&str,pos,prevPos]( ) { return Token::View{ &str[prevPos], pos - prevPos }; };
#ifndef APP_INNER // preserve token spaces and comments only for internal builds
         if ( kind == Kind::Space or kind == Kind::Comment ) continue;
#endif
         tokens.emplace_back( view( ), curLine, prevCol, kind );
#ifdef APP_ERROR
         cxx::cout << to_string( tokens.back( ) ) << std::endl;
#endif
      }
      if ( bufferBraces.idx != 0 ) throw Error{ "T?: braces depth not equal zero "s + bufferBraces.datastr( ) + " near "s + to_string( tokens.back( ) ) };
      tokens.emplace_back( "\0"sv, 0, 0, Kind::Last_ );
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
#ifdef APP_INNER
         case Kind::Comment: continue;
         case Kind::Space: continue;
#endif//APP_INNER
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
