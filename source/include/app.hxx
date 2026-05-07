#ifndef APP_HXX
#define APP_HXX

#include <son8/c/base.hxx>

#define APP_PROC void
#define APP_DATA static constexpr auto

namespace app {
   // type aliases
   using Char = unsigned char;
   using Size = son8::c::size_t;
   // maximum limits
   class Max final {
      APP_DATA s = 0b1ull;
   public:
      APP_DATA File_Size = Size{ s << 20u }; // 1 MiB
      APP_DATA print( unsigned long long i ) -> char const * {
         switch ( i ) {
            case 1'048'576u: return "1 MiB";
            default: return "error";
         }
      }
   };
} // namespace app

#endif//APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
