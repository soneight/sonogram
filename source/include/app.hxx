#ifndef APP_HXX
#define APP_HXX
// son8
// -- c
#include <son8/c/base.hxx>
// -- cxx
#include <son8/cxx/data.hxx>
#include <son8/cxx/file.hxx>
#include <son8/cxx/flow.hxx>
#include <son8/cxx/func.hxx>
#include <son8/cxx/text.hxx>
// -- mainland
#include <son8/main.hxx>
// -- core_lib
#include <son8/core.hxx>
// macros
#define APP_ASSERT assert
#define APP_CAST( type, value ) static_cast< type >( value )
#define APP_DATA static constexpr auto
#define APP_FUNC [[nodiscard]] inline auto
#define APP_LOCALE cxx::locale::classic( )
#define APP_PROC inline void

namespace app {
   using namespace son8;
   using namespace son8::core; // for Ptr, Out, Uni, Ref
   using namespace cxx::string_view_literals;
   // type aliases
   // -- fundamental
   using Char = unsigned char;
   using Size = c::size_t;
   using Unt0 = c::uint8_t;
   // -- classes
   template< typename Tp, Size Sz >
   using Array = cxx::array< Tp, Sz >;
   using Err = cxx::runtime_error;
   using String = cxx::string;
   using StringView = cxx::string_view;
   // maximum limits
   class Max final {
      APP_DATA s = 0b1ull;
   public:
      APP_DATA File_Size = Size{ s << 20u };    // 1 MiB
      APP_DATA Line_Length = Size{ s << 11u };  // 2 KiB
      APP_DATA print( unsigned long long i ) -> StringView {
         switch ( i ) {
            case 1'048'576u: return "1 MiB"sv;
            case 2'048u: return "2 KiB"sv;
            default: APP_ASSERT( false and "unreachable" ); return "error"sv;
         }
      }
   };

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

   struct Token final {
      enum class Kind : Unt0 {
         Space,
         Comment,
         Identifier,
         // NOTE app types must be contiguous
         APPTYPES_beg, // skip
         Apptype_Unknown = APPTYPES_beg,
         Apptype_Program,
         APPTYPES_end, // skip
         // NOTE keywords must be contiguous
         KEYWORDS_beg = APPTYPES_end, // skip
         Keyword_Program = KEYWORDS_beg,
         KEYWORDS_end, // skip
         // NOTE singles must be contiguous
         SINGLES_beg = KEYWORDS_end,
         Scope_Opened = SINGLES_beg,
         Scope_Closed,
         SINGLES_end, // skip
         // NOTE must be last
         Last_
      };
      using View = StringView;
      static_assert( Kind::SINGLES_end == Kind{ 8 } );
      // keywords helpers
      struct Keywords final {
         APP_DATA Beg = APP_CAST( Size, Kind::KEYWORDS_beg );
         APP_DATA End = APP_CAST( Size, Kind::KEYWORDS_end );
         APP_DATA Count = End - Beg;
         using Array_ = Array< View, Count >;
         APP_DATA Data = Array_{{
            "-program"sv
         }};
         static_assert( Count == Data.size( ) );
         static auto view_to_kind( View view ) -> Kind {
            auto it = cxx::find( Data.begin( ), Data.end( ), view );
            if ( it == Data.end( ) ) throw Err{ "app: tokens unknown keyword" };
            auto index = cxx::distance( Data.begin( ), it );
            return APP_CAST( Kind, Beg + index );
         }
      };

      // data members
      using Ref = app::Ref< Token >;
      View view;
      Size line;
      Size coln;
      Kind kind;
      // construtors
      Token( View view, Size line, Size coln, Kind kind )
      : view{ view }, line{ line }, coln{ coln }, kind{ kind } {  }
   };

   class Locale final {
      static inline auto &Facet_ = cxx::use_facet< cxx::ctype< char > >( APP_LOCALE );
   public:
      static bool is_alnum( Char ch ) { return Facet_.is( cxx::ctype_base::alnum, ch ); }
      static bool is_alpha( Char ch ) { return Facet_.is( cxx::ctype_base::alpha, ch ); }
      static bool is_blank( Char ch ) { return Facet_.is( cxx::ctype_base::blank, ch ); }
   };

   using Tokens = cxx::vector< Token >;

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
         if ( next_char( ) and not Locale::is_blank( ch ) ) throw Err{ "app::lex_tokens: no space character after comment symbol" };
         while ( next_char( ) and not ( ch == '\n' ) );
         return Token::Kind::Comment;
      };
      auto scan_keyword = [&]( ) {
         auto prevPos = pos - 1;
         while ( next_char( ) and Locale::is_alpha( ch ) );
         auto kind = Token::Keywords::view_to_kind( str.substr( prevPos, pos - prevPos - 1 ) );
         bool isApp = ( kind == Token::Kind::Keyword_Program );
         if ( isApp and tokens[0].kind != Token::Kind::Apptype_Unknown ) throw Err{ "application type duplicate" };
         else tokens[0] = Token{ "\0"sv, 0, 0, Token::Kind::Apptype_Program };
         return kind;
      };
      auto scan_singles = [&]( ) {
         Token::Kind kind;
         switch ( ch ) {
         case ':': kind = Token::Kind::Scope_Opened; break;
         case ';': kind = Token::Kind::Scope_Closed; break;
            default: throw Err{ "app::lex_tokens: unknown character to process" };
         }
         next_char( );
         return kind;
      };

      while ( next_char( ) ) {
         if ( ch == '\n' ) {
            Token::Ref prevToken = tokens.back( );
            if ( prevToken.kind == Token::Kind::Space ) throw Err{ "app::lex_tokens: trailing whitespace" };
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

      return tokens;
   }

   APP_FUNC to_string( Token::Kind kind ) -> String {
      using TokenView = Array< Token::View, APP_CAST( Size, Token::Kind::Last_ ) >;
      TokenView kinds{{
         "Spaces"sv,
         "Comment"sv,
         "Identifier"sv,
         "Application Type Unknown"sv,
         "Application Type Program"sv,
         "Keyword Program"sv,
         "Single Scope Begin"sv,
         "Single Scope End"sv,
         "Error: Unknown Token"sv
      }};
      return String{ kinds[APP_CAST(Size, kind)] };
   }

   APP_FUNC to_string( Token const &token ) -> String {
      String result;
      auto cb = cxx::to_string( token.coln );
      auto ce = cxx::to_string( token.coln + token.view.size( ) - 1 );
      result += "Token{";
      result += " Kind: " + to_string( token.kind );
      result += ", View:\"" + String{ token.view } + '"';
      result += " @" + cxx::to_string( token.line ) + ':' + cb + '-' + ce;
      result += " }";
      return result;
   }
} // namespace app

#endif//APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
