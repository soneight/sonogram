#ifndef APP_HXX
#define APP_HXX
// son8
// -- c
#include <son8/c/base.hxx>
// -- cxx
#include <son8/cxx/file.hxx>
#include <son8/cxx/flow.hxx>
#include <son8/cxx/text.hxx>
// -- mainland
#include <son8/main.hxx>
// -- core_lib
#include <son8/core.hxx>
// macros
#define APP_PROC void
#define APP_DATA static constexpr auto

namespace app {
   using namespace son8;
   using namespace son8::core; // for Ptr, Out, Uni, Ref

   // type aliases
   // -- fundamental
   using Char = unsigned char;
   using Size = c::size_t;
   // -- classes
   using String = cxx::string;
   // maximum limits
   class Max final {
      APP_DATA s = 0b1ull;
   public:
      APP_DATA File_Size = Size{ s << 20u };    // 1 MiB
      APP_DATA Line_Length = Size{ s << 11u };  // 2 KiB
      APP_DATA print( unsigned long long i ) -> char const * {
         switch ( i ) {
            case 1'048'576u: return "1 MiB";
            case 2'048u: return "2 KiB";
            default: return "error";
         }
      }
   };

   class Source final {
      bool assigned_{ };
      String data_;
   public:
      // default
      Source( ) = default;
     ~Source( ) = default;
      // delete
      Source( Uni< Source > move ) = delete;
      Source( Ref< Source > copy ) = delete;
      auto operator=( Ref< Source > copy ) = delete;
      // impl
      Source( Uni< String > str ) noexcept : data_{ son8::cxx::move( str ) } { }
      void operator=( Uni< Source > move ) {
         if ( assigned_ ) throw cxx::runtime_error{ "Source could be assigned only once" };
         data_ = cxx::move( move.data_ );
         assigned_ = true;
      }
      // a
      Ref< String > get( ) const { return data_; }
   };
} // namespace app

#endif//APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
