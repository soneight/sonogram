#include "face/alias/flow.hxx"
#include "face/program.hxx"

namespace app {

   void Token::throw_error( Ref< Text > text ) const {
      throw Error{ text + ": "s + to_string( *this ) };
   }

   using Tokens = Grow< Token >;
   APP_FUNC gen_program( Ref< Tokens > tokens ) -> Program {
      using Kind = Token::Kind;
      using State = Program::State;
      static constexpr int Scope_Opened = 0;
      static constexpr int Scope_Closed = 1;
      Program program;

      Token token = tokens.front( );
      APP_ASSERT( token.kind == Kind::Apptype_Program and "app::gen_program: tokens does not start with application type program" );
      token = tokens.back( );
      APP_ASSERT( token.kind == Kind::Last_ and "app::gen_program: tokens does not ends with last terminator" );
      auto itPos = tokens.begin( ) + 1;

      auto next_token = [&token,&itPos] {
         token = *itPos++;

         return token.kind != Kind::Last_;
      };

      auto peek_token = [&tokens,&itPos]( int offset = 0 ) {
         if ( itPos + offset < tokens.end( )) return *( itPos + offset );
         return tokens.back( );
      };

      Text s{ "gen_program: " };
      Text currIdentifier;
      Bool isProgramExist{ };

      auto parse_global = [&] {
         switch ( token.kind ) {
         case Kind::Identifier: { currIdentifier = token.view; return; }
         case Kind::Keyword_Program: {
            if ( currIdentifier.empty( )) { token.throw_error( s + "no identifier in global state before"s ); }
            if ( isProgramExist ) token.throw_error( s + "program repeated"s );
            isProgramExist = true;
            program.fileName = currIdentifier;
            return;
         }
         case Kind::Scope_Opened: {
            if ( currIdentifier.empty( )) { token.throw_error( s + "no identifier in global state before"s ); }
            if ( not isProgramExist ) { token.throw_error( s + "no program keyword in global state before"s ); }
            currIdentifier.clear( );
            program.state = State::Body;
            return;
         }
            default: { token.throw_error( s + "unexpected token in global state" ); }
         }
      };

      auto parse_body = [&] {
         switch ( token.kind ) {
         case Kind::Scope_Closed: {
            return;
         }
            default: token.throw_error( s + "unexpected token in body state" );
         }
      };



      while ( next_token( ) ) {
#ifdef APP_INNER
         if ( token.kind == Kind::Space or token.kind == Kind::Comment ) continue;
#endif//APP_INNER

         switch ( program.state ) {
         case State::Global: parse_global( ); continue;
         case State::Body: parse_body( ); continue;
            default: {
               token.throw_error( "gen_program: unhandled program state" );
            }
         }
      } // while next_token( )

      if ( program.state == State::Global ) { throw Error{ s + "incorrect state on last token"s }; }

      return program;
   }
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
