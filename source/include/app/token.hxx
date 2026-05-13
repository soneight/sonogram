#ifndef APP_TOKEN_HXX
#define APP_TOKEN_HXX

#include <app/alias.hxx>
// son8
// -- cxx_unit
#include <son8/cxx/func.hxx>

namespace app {
   struct Token final {
      enum class Kind : Unt0 {
         Space,
         Comment,
         Identifier,
         // NOTE app types must be contiguous
         APPTYPES_beg, // skip
         Apptype_Unknown = APPTYPES_beg,
         Apptype_Program,
         APPTYPES_end, // skip
         // NOTE keywords must be contiguous
         KEYWORDS_beg = APPTYPES_end, // skip
         Keyword_Program = KEYWORDS_beg,
         KEYWORDS_end, // skip
         // NOTE singles must be contiguous
         SINGLES_beg = KEYWORDS_end,
         Scope_Opened = SINGLES_beg,
         Scope_Closed,
         SINGLES_end, // skip
         Error = SINGLES_end,
         // NOTE must be last
         Last_
      };
      APP_DATA Count = APP_CAST( Size, Kind::Last_ );
      static_assert( Kind::Last_ == Kind{ Count } );
      using View = StringView;
      // keywords helpers
      struct Keywords final {
         APP_DATA Beg = APP_CAST( Size, Kind::KEYWORDS_beg );
         APP_DATA End = APP_CAST( Size, Kind::KEYWORDS_end );
         APP_DATA Count = End - Beg;
         using Array = Flat< View, Count >;
         APP_DATA Data = Array{{
            "-program"sv
         }};
         static_assert( Count == Data.size( ) );
         static auto view_to_kind( View view ) -> Kind {
            auto it = cxx::find( Data.begin( ), Data.end( ), view );
            if ( it == Data.end( ) ) throw Error{ "app: tokens unknown keyword" };
            auto index = cxx::distance( Data.begin( ), it );
            return APP_CAST( Kind, Beg + index );
         }
      };

      // data members
      using Ref = app::Ref< Token >;
      View view;
      Size line;
      Size coln;
      Kind kind;
      // constructors
      // Token( ) = default;
      Token( View view, Size line, Size coln, Kind kind )
      : view{ view }, line{ line }, coln{ coln }, kind{ kind } {  }
   };
}

#endif//APP_TOKEN_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
