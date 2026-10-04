#ifndef FACE_TOKENIZER_HXX
#define FACE_TOKENIZER_HXX

#include "token.hxx"

namespace app {

   APP_DCLP tokens_clear( );

   APP_DCLP tokens_reserve( Size size );

   APP_DCLF tokens_size( ) -> Token::Index;

   APP_DCLF tokens_back( ) -> Token;

   APP_DCLP tokens_append( Uni< Token::Item > item );

   APP_DCLP lex_tokens( Ref< Text > str );

}

#endif//FACE_TOKENIZER_HXX
