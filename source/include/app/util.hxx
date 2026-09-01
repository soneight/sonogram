#ifndef APP_UTIL_HXX
#define APP_UTIL_HXX

#include "alias.hxx"
#include "token.hxx"

namespace app {

   APP_FUNC to_string( Token::Kind kind ) -> String {
      using TokenView = Flat< Token::View, Token::Count + 1 >;
      static constexpr TokenView kinds{{
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
      return String{ kinds[APP_CAST( Size, kind )] };
   }

   APP_FUNC to_string( Token const &token ) -> String {
      String result;
      auto cb = cxx::to_string( token.coln );
      auto ce = cxx::to_string( token.coln + token.view.size( ) - 1 );
      result += "Token{";
      result += " Kind: " + to_string( token.kind );
      if ( token.view[0] == '"' ) result += ", View: '" + String{ token.view } + "'";
      else result += ", View: \"" + String{ token.view } + '"';
      result += " @" + cxx::to_string( token.line ) + ':' + cb + '-' + ce;
      result += " }";
      return result;
   }

}

#endif//APP_UTIL_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
