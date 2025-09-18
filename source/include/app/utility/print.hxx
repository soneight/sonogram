#ifndef APP_UTILITY_PRINT_HXX
#define APP_UTILITY_PRINT_HXX

#include <app/tokenizer/token.hxx>

#include <iosfwd>

namespace app {

    std::ostream &operator<<( std::ostream &os, Token::Kind kind );
    std::ostream &operator<<( std::ostream &os, Token const &token );

}

#endif//APP_UTILITY_PRINT_HXX
