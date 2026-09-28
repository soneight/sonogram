#ifndef FACE_LOCALE_HXX
#define FACE_LOCALE_HXX

#include "alias/pour.hxx"

namespace app {
   class Locale final {
      APP_DATA Alnums = 0u;
      APP_DATA Alphas = 1u;
      APP_DATA Binary = 2u;
      APP_DATA Blanks = 3u;
      APP_DATA Digits = 4u;
      APP_DATA Prints = 5u;
      using InnerMasks_ = Flat< Unt3, 2 >;
      using AllMasks_ = Flat< InnerMasks_, 6 >;
      APP_DATA Masks = AllMasks_{{
         { 0x03FF'0000'0000'0000ull, 0x07FF'FFFE'07FF'FFFEull }, // Alnums: 57-48(9-0), 122-97(z-a) 90-65(Z-A)
         { 0x0000'0000'0000'0000ull, 0x07FF'FFFE'07FF'FFFEull }, // Alphas: 122-97(z-a) 90-65(Z-A)
         { 0x7000'0040'0000'0000ull, 0x5000'0000'0000'0000ull }, // Binary: 62-60(>=<) 38(&), 126(~) 124(|)
         { 0x0000'0001'0000'0200ull, 0x0000'0000'0000'0000ull }, // Blanks: 20(\ ) 9(\t)
         { 0x03FF'0000'0000'0000ull, 0x0000'0000'0000'0000ull }, // Digits: 57-48(9-0)
         { 0xFFFF'FFFF'0000'0000ull, 0x7FFF'FFFF'FFFF'FFFFull }, // Prints
      }};
      APP_DATA dbg_( Char ch ) { APP_ASSERT( ch < 128u ); }
   public:
      APP_DATA is_alnums( Char ch ) -> bool;
      APP_DATA is_alphas( Char ch ) -> bool;
      APP_DATA is_binary( Char ch ) -> bool;
      APP_DATA is_blanks( Char ch ) -> bool;
      APP_DATA is_digits( Char ch ) -> bool;
      APP_DATA is_prints( Char ch ) -> bool;
   };
}

#endif//FACE_LOCALE_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
