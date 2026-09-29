#ifndef FACE_APP_HXX
#define FACE_APP_HXX
// face
#include "alias.hxx"
#include "limits.hxx"
#include "program.hxx"
#include "tokenizer.hxx"
// son8
#include <son8/core/alias/data.hxx>

namespace app {
   // TODO: this is temporary solution for global source string handling
   class Source final {
      bool assigned_{ };
      Text data_;
   public:
      // default
      Source( ) = default;
     ~Source( ) = default;
      // delete
      Source( Uni< Source > move ) = delete;
      Source( Ref< Source > copy ) = delete;
      APP_PROC operator=( Ref< Source > copy ) = delete;
      // impl
      Source( Uni< Text > str ) noexcept : data_{ cxx::move( str )} { }
      APP_DISC operator=( Uni< Source > move ) noexcept -> Out< Source > {
         assert( not assigned_ and "Source could be assigned only once" );
         data_ = cxx::move( move.data_ );
         assigned_ = true;
         return *this;
      }
      // accessors
      APP_FUNC get( ) -> Out< Text > { return data_; }
      APP_FUNC get( ) const -> Ref< Text > { return data_; }
   };

}

#endif//FACE_APP_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
