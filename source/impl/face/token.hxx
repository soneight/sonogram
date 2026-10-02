#ifndef FACE_TOKEN_HXX
#define FACE_TOKEN_HXX

#include "alias/text.hxx"

namespace app {
   struct Token final {
      enum class Kind : Unt0 {
         Space,
         Comment,
         Identifier,
         Number,
         Unary_Plus,
         // NOTE app types must be contiguous
         APPTYPES_beg, // skip
         Apptype_Unknown = APPTYPES_beg,
         Apptype_Program,
         // TODO: Apptype_Package,
         // TODO: AppType_Library,
         // TODO(maybe?): AppType_Console,
         APPTYPES_end, // skip
         // NOTE keywords must be contiguous
         KEYWORDS_beg = APPTYPES_end, // skip
         Keyword_Program = KEYWORDS_beg,
         Keyword_Echo,
         Keyword_Char,
         Keyword_Int2,
         Keyword_Int3,
         Keyword_Half,
         Keyword_Real,
         Keyword_Void,
         Keyword_For,
         Keyword_Case,
         Keyword_And,
         Keyword_Or,
         Keyword_In,
         Keyword_Else,
         Keyword_Cast,
         Keyword_Expr,
         Keyword_Func,
         Keyword_Exit,
         KEYWORDS_end, // skip
         // NOTE singles must be contiguous, and open closed
         // \ brackets must be placed adjacent to each other
         SINGLES_beg = KEYWORDS_end,
         Scope_Opened = SINGLES_beg,
         Scope_Closed,
         Curly_Opened, // Expr
         Curly_Closed, // Expr
         Round_Opened, // Func
         Round_Closed, // Func
         Array_Opened,
         Array_Closed,
         Single_Comma,
         Literal_Single,
         Literal_Double,
         Literal_Grave,
         // Literal_Grave,
         SINGLES_end, // skip
         // NOTE binary must be contiguous
         BINARY_beg = SINGLES_end,
         Less_Equal = BINARY_beg,
         Less_Less,
         Equal_Equal,
         More_More,
         Not_Equal,
         Math_Multiply,
         Math_Divide,
         Math_Plus,
         Math_Minus,
         BINARY_end, // skip
         Error = BINARY_end,
         // NOTE must be last
         Last_
      };
      APP_DATA is_bracket_opened( Kind kind ) -> bool;
      APP_DATA Count = APP_CAST( Size, Kind::Last_ );
      static_assert( Kind::Last_ == Kind{ Count });
      // keywords helpers
      struct Keywords final {
         APP_DATA Beg = APP_CAST( Int2, Kind::KEYWORDS_beg );
         APP_DATA End = APP_CAST( Int2, Kind::KEYWORDS_end );
         APP_DATA Count = End - Beg;
         using Array = Flat< View, Count >;
         APP_DATA Data = Array{{
            "-program"sv,
            "-echo"sv,
            "-char"sv,
            "-int2"sv,
            "-int3"sv,
            "-half"sv,
            "-real"sv,
            "-void"sv,
            "-for"sv,
            "-case"sv,
            "-and"sv,
            "-or"sv,
            "-in"sv,
            "-else"sv,
            "-cast"sv,
            "-expr"sv,
            "-func"sv,
            "-exit"sv,
         }};
         static_assert( Count == Data.size( ));
         static auto view_to_kind( View view ) -> Kind;
      };
      using Line = Unt2;
      using Coln = Unt2;
      // data members
      View view;
      Line line;
      Coln coln;
      Kind kind;
      // constructors
      // Token( ) = default;
      Token( View view, Line line, Coln coln, Kind kind )
      : view{ view }, line{ line }, coln{ coln }, kind{ kind } {  }
      // throw current token on error
      void throw_error( Ref< Text > text ) const;
      Text str_literal( ) const {
         APP_ASSERT( kind == Kind::Literal_Single and "require literal to process" );
         return Text{ view.substr( 1, view.size( ) - 2 ) };
      }
   }; // struct Token

   APP_FUNC to_string( Token::Kind kind ) -> Text;
   APP_FUNC to_string( Ref< Token > token ) -> Text;
}

#endif//FACE_TOKEN_HXX

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
