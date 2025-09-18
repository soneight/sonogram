#ifndef APP_UTILITY_PRINT_HXX
#define APP_UTILITY_PRINT_HXX

#include <app/tokenizer/token.hxx>

#include <iosfwd>

namespace app {

    auto to_string( Token::Kind kind ) -> Token::Value;

    std::ostream &operator<<( std::ostream &os, Token::Kind kind );
    std::ostream &operator<<( std::ostream &os, Token const &token );

}

#endif//APP_UTILITY_PRINT_HXX
