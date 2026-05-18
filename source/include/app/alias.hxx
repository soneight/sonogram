#ifndef APP_ALIAS_HXX
#define APP_ALIAS_HXX
// son8
// -- cxx_unit
#include <son8/cxx/flow.hxx>
#include <son8/cxx/text.hxx>
// -- core_lib
#include <son8/core.hxx>

// macros
#define APP_LOCALE cxx::locale::classic( )
// -- seperate app asserts from std asserts
#ifdef APP_DEBUG
#  define APP_ASSERT assert
#else
#  define APP_ASSERT( ignore ) ((void)0)
#endif//APP_DEBUG
#define APP_CAST( type, value ) static_cast< type >( ( value ) )
#define APP_DATA static constexpr auto
#define APP_FUNC [[nodiscard]] inline auto
#define APP_PROC inline void

namespace app {
   using namespace son8;
   using namespace cxx::string_literals;
   using namespace cxx::string_view_literals;

   using namespace son8::core; // for Ptr, Out, Uni, Ref

   // type aliases
   template< typename Type, unsigned Size >
   using Flat = Array< Type, Size >;
   using Error = cxx::runtime_error;
   using String = cxx::string;
   using StringView = cxx::string_view;
} // namespace app

#endif//APP_ALIAS_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
