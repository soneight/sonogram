#ifndef APP_UTILITY_PRINT_HXX
#define APP_UTILITY_PRINT_HXX

#ifndef SON8_SONOGRAM_INCLUDE_OLD_IMPLEMENTATION
#error "File is subject to be removed. Use source/face/app/utility/print.hxx instead."
#endif

#include <app/tokenizer/token.hxx>

#include <iosfwd>
#include <string>

namespace app {

    auto to_string( Token::Kind kind ) -> std::string;
    auto to_string( Token const &token ) -> std::string;

    std::ostream &operator<<( std::ostream &os, Token::Kind kind );
    std::ostream &operator<<( std::ostream &os, Token const &token );

}

#endif//APP_UTILITY_PRINT_HXX
