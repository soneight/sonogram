#ifndef APP_LOCALE_HXX
#define APP_LOCALE_HXX

#include <app/alias.hxx>
#include <son8/cxx/text.hxx>

namespace app {
   class Locale final {
      static inline auto &Facet_ = cxx::use_facet< cxx::ctype< char > >( APP_LOCALE );
   public:
      static bool is_alnum( Char ch ) { return Facet_.is( cxx::ctype_base::alnum, ch ); }
      static bool is_alpha( Char ch ) { return Facet_.is( cxx::ctype_base::alpha, ch ); }
      static bool is_blank( Char ch ) { return Facet_.is( cxx::ctype_base::blank, ch ); }
      static bool is_digit( Char ch ) { return Facet_.is( cxx::ctype_base::digit, ch ); }
      static bool is_print( Char ch ) { return Facet_.is( cxx::ctype_base::print, ch ); }
      static bool is_binary(Char ch ) {
         return ch == '<'
             or ch == '>'
             or ch == '='
             or ch == '*'
             or ch == '/'
             or ch == '+'
             or ch == '-';
      }
   };
} // namespace app

#endif//APP_LOCALE_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
