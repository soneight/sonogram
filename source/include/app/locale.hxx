#ifndef APP_LOCALE_HXX
#define APP_LOCALE_HXX

#include "alias.hxx"

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
      APP_DATA is_alnums( Char ch ) -> bool { dbg_( ch ); return ( Masks[Alnums][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
      APP_DATA is_alphas( Char ch ) -> bool { dbg_( ch ); return ( Masks[Alphas][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
      APP_DATA is_binary( Char ch ) -> bool { dbg_( ch ); return ( Masks[Binary][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
      APP_DATA is_blanks( Char ch ) -> bool { dbg_( ch ); return ( Masks[Blanks][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
      APP_DATA is_digits( Char ch ) -> bool { dbg_( ch ); return ( Masks[Digits][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
      APP_DATA is_prints( Char ch ) -> bool { dbg_( ch ); return ( Masks[Prints][ch >> 6u] >> ( ch & 63u ) ) & 1u; }
   };

   static_assert( Locale::is_alnums( '0' ) == true );
   static_assert( Locale::is_alnums( '9' ) == true );
   static_assert( Locale::is_alnums( 'A' ) == true );
   static_assert( Locale::is_alnums( 'Z' ) == true );
   static_assert( Locale::is_alnums( 'a' ) == true );
   static_assert( Locale::is_alnums( 'z' ) == true );
   static_assert( Locale::is_alphas( 'A' ) == true );
   static_assert( Locale::is_alphas( 'Z' ) == true );
   static_assert( Locale::is_alphas( 'a' ) == true );
   static_assert( Locale::is_alphas( 'z' ) == true );
   static_assert( Locale::is_binary( '&' ) == true );
   static_assert( Locale::is_binary( '<' ) == true );
   static_assert( Locale::is_binary( '=' ) == true );
   static_assert( Locale::is_binary( '>' ) == true );
   static_assert( Locale::is_binary( '|' ) == true );
   static_assert( Locale::is_binary( '~' ) == true );
   static_assert( Locale::is_binary( 127 ) ==false );
   static_assert( Locale::is_binary( 0x0 ) ==false );
   static_assert( Locale::is_binary( 010 ) ==false ); // 8
   static_assert( Locale::is_binary(0100 ) ==false ); // 64
   static_assert( Locale::is_blanks( ' ' ) == true );
   static_assert( Locale::is_blanks('\t' ) == true );
   static_assert( Locale::is_digits( '0' ) == true );
   static_assert( Locale::is_digits( '9' ) == true );
} // namespace app

#endif//APP_LOCALE_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
