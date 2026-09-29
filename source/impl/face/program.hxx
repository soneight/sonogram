#ifndef FACE_PROGRAM_HXX
#define FACE_PROGRAM_HXX

#include "alias/text.hxx"
#include "alias/data.hxx"
#include "token.hxx"

namespace app {
   // program
   struct Program final {
      // states
      enum class State : Unt0 {
         Global,
         Body,
      };
      // data members
      State state{ };
      Text fileName{ };
      Text mainHead;
      Text mainBody{ };
      Text mainFoot;
      Program( )
      : fileName{ "program" }
      , mainHead{ "int main( ) {" }
      , mainFoot{ "}" } { }
   };

   using Tokens = Grow< Token >;
   APP_FUNC gen_program( Ref< Tokens > tokens ) -> Program;

}

#endif//FACE_PROGRAM_HXX
