#include "face/program.hxx"
#include "face/alias/data.hxx"
#include "face/alias/flow.hxx"

namespace app {
   using Tokens = Grow< Token >;
   APP_FUNC gen_program( Ref< Tokens > tokens ) -> Program {
      using Kind = Token::Kind;
      using State = Program::State;
      constexpr int Scope_Opened = 0;
      constexpr int Scope_Closed = 1;
      Program program;
      // auto const itEnd = tokens.end( );
      auto itPos = tokens.begin( ) + 1;
      Kind expectKind = Kind::Identifier;
      Token token = tokens.back( );

      if ( token.kind != Kind::Last_ ) { throw Error{ "app::gen_program: tokens does not ends with last terminator" }; }

      auto next_token = [&]( ) -> bool {
         token = *itPos++;

         return token.kind != Kind::Last_;
      };
      int scopeDepth{ };
      auto parse_scope = [&]( int scope ) {
         if ( program.state == State::Global and expectKind != Kind::Scope_Opened ) { throw Error{ "app::gen_program expect open score in global state" }; }

         program.state = State::Body;
         int d[2] = { scopeDepth + 1, scopeDepth - 1 };
         scopeDepth = d[scope];

         if ( scopeDepth < 0 ) { throw Error{ "app::gen_program: scope depth negative" }; }
      };
      auto parse_program = [&]( ) {
         if ( program.state == State::Global ) {
            if ( expectKind != Kind::Keyword_Program ) { throw Error{ "app::gen_program: expect keyword program in global state" }; }

            expectKind = Kind::Scope_Opened;
         }
      };
      auto parse_identifier = [&]( ) {
         if ( program.state == State::Global ) {
            program.fileName = token.view;
            expectKind = Kind::Keyword_Program;
         }
      };

      while ( next_token( ) ) {
         switch ( token.kind ) {
#ifdef APP_INNER
         case Kind::Comment: continue;
         case Kind::Space: continue;
#endif//APP_INNER
         case Kind::Identifier: parse_identifier( ); continue;
         case Kind::Keyword_Program: parse_program( ); continue;
         case Kind::Scope_Opened: parse_scope( Scope_Opened ); continue;
         case Kind::Scope_Closed: parse_scope( Scope_Closed ); continue;
         case Kind::Last_: break;
            default: {
                throw Error{ "app::gen_program: token is not supported yet" + to_string( token )};
            }
         }

         switch ( program.state ) {
            default: continue;
         }
      }

      if ( scopeDepth ) throw Error{ "app::gen_program: scope depth not equal zero: " + cxx::to_string( scopeDepth)};

      return program;
   }
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
