#ifndef FACE_ALIAS_POUR_HXX
#define FACE_ALIAS_POUR_HXX

#include <son8/core/alias/pour.hxx>
// macros
// -- separate app asserts from std asserts
#ifdef APP_DEBUG
#  define APP_ASSERT assert
#else
#  define APP_ASSERT( ignore ) ((void)0)
#endif//APP_DEBUG
#define APP_CAST( type, value ) static_cast< type >(( value ))
#define APP_DATA static constexpr auto
#define APP_EXPR constexpr auto
#define APP_DISC auto
#define APP_FUNC [[nodiscard]] auto
#define APP_PROC inline void

namespace app {
   using namespace son8;
   using namespace core;
}

#endif//FACE_ALIAS_POUR_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
