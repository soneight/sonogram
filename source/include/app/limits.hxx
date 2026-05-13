#ifndef APP_LIMITS_HXX
#define APP_LIMITS_HXX

#include <app/alias.hxx>

namespace app {
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
} // namespace son8

#endif//APP_LIMITS_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
