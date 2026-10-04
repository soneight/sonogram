#include "face/app.hxx"
// son8
#include <son8/cxx/func.hxx>

namespace app {

   APP_FUNC to_string( Token::Kind kind ) -> Text {
      using TokenView = Flat< View, Token::Count + 1 >;
      APP_DATA kinds = TokenView{{
         "Spaces"sv,
         "Comment"sv,
         "Identifier"sv,
         "Number"sv,
         "Unary Plus"sv,
         "Application Type Unknown"sv,
         "Application Type Program"sv,
         "Keyword program"sv,
         "Keyword echo"sv,
         "Keyword char"sv,
         "Keyword int2 Type 32-bit Integer"sv,
         "Keyword int3 Type 64-bit Integer"sv,
         "Keyword half Type 32-bit Float"sv,
         "Keyword real Type 64-bit Double"sv,
         "Keyword void"sv,
         "Keyword for"sv,
         "Keyword case"sv,
         "Keyword and"sv,
         "Keyword or"sv,
         "Keyword in"sv,
         "Keyword else"sv,
         "Keyword cast"sv,
         "Keyword expr"sv,
         "Keyword func"sv,
         "Keyword exit"sv,
         "Single Scope Begin"sv,
         "Single Scope End"sv,
         "Single Expr Begin"sv,
         "Single Expr End"sv,
         "Single Func Begin"sv,
         "Single Func End"sv,
         "Single Array Begin"sv,
         "Single Array End"sv,
         "Single Comma"sv,
         "Literal Single"sv,
         "Literal Double"sv,
         "Literal Grave"sv,
         "Binary Less Equal"sv,
         "Binary Less Less"sv,
         "Binary Equal Equal"sv,
         "Binary More More"sv,
         "Binary Not Equal"sv,
         "Binary Math Multiply"sv,
         "Binary Math Divide"sv,
         "Binary Math Plus"sv,
         "Binary Math Minus"sv,
         "Error: Unknown Token"sv,
         "App Terminator"sv
      }};

      return Text{ kinds[APP_CAST( Size, kind )] };
   }

   auto Token::Keywords::view_to_kind( View view ) -> Kind {
      auto it = cxx::find( Data.begin( ), Data.end( ), view );

      if ( it == Data.end( ) ) { return Kind::Error; }

      auto index = cxx::distance( Data.begin( ), it );

      return APP_CAST( Kind, Beg + index );
   }

   APP_FUNC to_string( Ref< Token > token ) -> Text {
      Text result;
      auto cb = cxx::to_string( token.coln( ));
      auto ce = cxx::to_string( token.coln( ) + token.view( ).size( ) - 1 );
      result += "Token{";
      result += " Kind: " + to_string( token.kind( ));

      if ( token.view( )[0] == '"' ) { result += ", View: '" + Text{ token.view( )} + "'"; }
      else { result += ", View: \"" + Text{ token.view( )} + '"'; }

      result += " @" + cxx::to_string( token.line( )) + ':' + cb + '-' + ce;
      result += " }";

      return result;
   }

}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
