#ifndef FACE_LIMITS_HXX
#define FACE_LIMITS_HXX

#include "alias/text.hxx"

namespace app {
   // maximum limits
   class Max final {
      APP_DATA s = 0b1u;
   public:
      APP_DATA File_Size = Size{ s << 20u };    // 1 MiB
      APP_DATA Line_Length = Size{ s << 11u };  // 2 KiB
      APP_DATA Scoped_Depth = Size{ s << 2u };  // 4 Bit
      APP_DATA Nested_Depth = Size{ s << 3u };  // 8 Bit
      APP_DATA print( unsigned long long i ) -> View {
         switch ( i ) {
         case 1'048'576u:  return "1 MiB"sv;
         case 2'048u:      return "2 KiB"sv;
         case 4u:          return "4 Bit"sv;
         case 8u:          return "8 Bit"sv;
            default: {
                APP_ASSERT( false and "unreachable" ); return "error"sv;
            }
         }
      }
      APP_DATA is_valid_file_size( Size value ) -> bool { return value <= File_Size; }
   };
} // namespace son8

#endif//FACE_LIMITS_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
