#ifndef APP_UTIL_HXX
#define APP_UTIL_HXX

#include <app/alias.hxx>
#include <app/token.hxx>

namespace app {

   APP_FUNC to_string( Token::Kind kind ) -> String {
      using TokenView = Flat< Token::View, Token::Count + 1 >;
      TokenView kinds{{
         "Spaces"sv,
         "Comment"sv,
         "Identifier"sv,
         "Application Type Unknown"sv,
         "Application Type Program"sv,
         "Keyword Program"sv,
         "Single Scope Begin"sv,
         "Single Scope End"sv,
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
      result += ", View:\"" + String{ token.view } + '"';
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
