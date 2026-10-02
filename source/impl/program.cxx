#include "face/alias/flow.hxx"
#include "face/program.hxx"

namespace app {

   APP_DATA Math_Compound_Ops = Flat< View, 4 >{{
      "*="sv, "/="sv, "+="sv, "-="sv
   }};
   APP_DATA math_compound_op( Token::Kind kind ) -> View {
      return Math_Compound_Ops[APP_CAST( Unt0, kind ) - APP_CAST( Unt0, Token::Kind::Math_Multiply )];
   }

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
      token = *itPos;
      auto next_token = [&token,&itPos] {
         token = *itPos++;

         return token.kind != Kind::Last_;
      };

      auto peek_token = [&tokens,&itPos]( int offset = 0 ) {
         if ( itPos + offset < tokens.end( )) return *( itPos + offset );
         return tokens.back( );
      };

      Text s{ "gen_program: " };
      View currIdentifier;
      Bool isProgramExist{ };
      Bool isVoidApplicable{ };

      auto parse_name = [&] {
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
            currIdentifier = {};
            program.state = State::Body;
            return;
         }
            default: { token.throw_error( s + "unexpected token in global state" ); }
         }
      };

      Tide< View > varTide;

      auto parse_body = [&] {
         switch ( token.kind ) {
         case Kind::Keyword_Echo: { program.state = State::Echo; return; }
         case Kind::Identifier: {
            if ( not currIdentifier.empty( )) { token.throw_error( s + "double identifier"s ); }
            currIdentifier = token.view;

            if ( varTide.count( currIdentifier )) { program.state = State::Init; return; }

            return;
         }
         case Kind::Keyword_Int2: {
            if ( currIdentifier.empty( )) { token.throw_error( s + "specifier requires identifier"s ); }

            if ( peek_token( ).kind != Kind::Single_Comma ) { isVoidApplicable = true; return; }

            program.mainBody.append( "\tint "s + Text{ currIdentifier } + " = 0;\n"s );
            varTide.insert( currIdentifier );
            currIdentifier = { };
            next_token( );

            return;
         }
         case Kind::Keyword_Void: {
            if ( not isVoidApplicable ) { token.throw_error( s + "not void applicable"s ); }

            if ( peek_token( ).kind != Kind::Single_Comma ) { token.throw_error( s + "void should be last specifier"s ); }

            program.mainBody.append( "\tint "s + Text{ currIdentifier } + ";\n"s );
            varTide.insert( currIdentifier );
            currIdentifier = { };
            isVoidApplicable = false;
            next_token( );

            return;
         }
         case Kind::Scope_Closed: {
            return;
         }
            default: token.throw_error( s + "unexpected token in body state" );
         }
      };

      auto parse_init = [&] {
         switch ( token.kind ) {
         case Kind::Number: {
            if ( peek_token( ).kind != Kind::Single_Comma ) { token.throw_error( s + "expected comma after init"s ); }

            program.mainBody.append( "\t"s + Text{ currIdentifier } + " = " + Text{ token.view } + ";\n" );
            program.state = State::Body;
            currIdentifier = { };
            next_token( );

            return;
         }
         case Kind::Identifier: {
            auto curr = token.view;
            if ( not varTide.count( curr ) ) { token.throw_error( s + "unknown identifier variable"s ); }

            if ( peek_token( ).kind != Kind::Single_Comma ) { token.throw_error( s + "expected comma after init"s ); }

            program.mainBody.append( "\t"s + Text{ currIdentifier } + " = "s + Text{ curr } + ";\n"s );
            program.state = State::Body;
            currIdentifier = { };
            next_token( );

            return;
         }
         case Kind::Math_Minus:
         case Kind::Math_Divide:
         case Kind::Math_Multiply:
         case Kind::Math_Plus: {
            auto peek = peek_token( );
            auto peek2 = peek_token( 1 );
            bool isNumberEnd = peek.kind == Kind::Number and peek2.kind == Kind::Single_Comma;
            if ( not isNumberEnd ) { token.throw_error( s + "expected number in body init state after math operator"s ); }
            program.mainBody.append( "\t"s + Text{ currIdentifier } + Text{ math_compound_op( token.kind ) } + Text{ peek.view } + ";\n" );
            program.state = State::Body;
            currIdentifier = { };
            next_token( );
            next_token( );

            return;
         }
            default: { token.throw_error( s + "unexpected token in init state"s ); }
         }
      };

      auto parse_echo = [&] {
         switch ( token.kind ) {
         case Kind::Literal_Single: {
            auto peek = peek_token( );
            if ( peek.kind != Kind::Single_Comma ) { token.throw_error( s + "bad token after string literal"s ); }
            program.mainBody.append( "\tstd::cout << \""s + token.str_literal( ) + "\";\n");
            program.state = State::Body;
            next_token( );
            return;
         }
         case Kind::Identifier: {
            if ( not varTide.count( token.view )) { token.throw_error( s + "unknown identifier in echo state"s ); }
            if ( peek_token( ).kind != Kind::Single_Comma ) { token.throw_error( s + "expect comma after identifier in echo state"s ); }
            program.mainBody.append( "\tstd::cout << "s + Text{ token.view } + ";\n" );
            program.state = State::Body;
            next_token( );
            return;
         }
            default: { token.throw_error( s + "unexpected token in body echo state"s ); }
         }
      };

      while ( next_token( ) ) {
#ifdef APP_INNER // only inner builds should preserve spaces and comments
         if ( token.kind == Kind::Space or token.kind == Kind::Comment ) continue;
#endif//APP_INNER

         switch ( program.state ) {
         case State::Name: parse_name( ); continue;
         case State::Body: parse_body( ); continue;
         case State::Init: parse_init( ); continue;
         case State::Echo: parse_echo( ); continue;
            default: {
               token.throw_error( "gen_program: unhandled program state" );
            }
         }
      } // while next_token( )

      if ( not currIdentifier.empty( )) { throw Error{ s + "identifier leftover: " + Text{ currIdentifier }}; }
      if ( program.state == State::Name ) { throw Error{ s + "incorrect state on last token"s }; }

      return program;
   }
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
