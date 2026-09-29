#ifndef FACE_TOKENIZER_HXX
#define FACE_TOKENIZER_HXX

#include "alias/data.hxx"
#include "token.hxx"

namespace app {

   using Tokens = Grow< Token >;
   APP_FUNC lex_tokens( Ref< Text > str ) -> Tokens;

}

#endif//FACE_TOKENIZER_HXX
