#ifndef APP_UTILITY_PRINT_HXX
#define APP_UTILITY_PRINT_HXX
// app headers
#include <app/lexer/token.hxx> // class Token
// std headers
#include <iosfwd> // std::ostream
#include <string> // std::string

namespace app {

    auto to_string( Token::Kind kind ) -> std::string;
    auto to_string( Token const &token ) -> std::string;

    auto operator<<( std::ostream &os, Token::Kind kind ) -> std::ostream &;
    auto operator<<( std::ostream &os, Token const &token ) -> std::ostream &;

} // namespace app

#endif//APP_UTILITY_PRINT_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
