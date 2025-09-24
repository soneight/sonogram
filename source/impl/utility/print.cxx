// own header
#include <app/utility/print.hxx>
// app headers
#include <app/alias.hxx> // Size
#include <app/lexer/token.hxx> // class Token
// std headers
#include <array> // std::array
#include <cassert> // assert
#include <iostream> // std::ostream
#include <string> // std::string
#include <string_view> // std::operator""sv

namespace app {

    auto to_string( Token::Kind kind ) -> std::string {
        using std::operator""sv;
        static constexpr std::array< Token::Value, static_cast< alias::Size >( Token::Kind::Last ) > kinds{{
            "IdentifierDefault"sv,
            "IdentifierCaps"sv,
            "LiteralString"sv,
            "KeywordProgram"sv,
            "KeywordEcho"sv,
            "ScopeBeg"sv,
            "ScopeEnd"sv,
            "Comma"sv,
            "Unknown"sv
        }};
        assert( kind < Token::Kind::Last );
        return std::string{ kinds[static_cast< alias::Size >( kind )] };
    }

    auto to_string( Token const &token ) -> std::string {
        std::string result;
        result += "Token{";
        if ( token.has_kind( ) ) result += " " + to_string( token.kind( ) );
        if ( token.has_value( ) ) {
            result += " ";
            if ( token.kind( ) == Token::Kind::LiteralString ) result += std::string{ token.value( ) };
            else result += "'" + std::string{ token.value( ) } + "'";
        }
        if ( token.has_position( ) ) result += " @" + std::to_string( token.line( ) ) + ":" + std::to_string( token.column( ) );
        result += " }";
        return result;
    }

    std::ostream &operator<<( std::ostream &os, Token::Kind kind ) {
        os << to_string( kind );
        return os;
    }

    std::ostream &operator<<( std::ostream &os, Token const &token ) {
        os << to_string( token );
        return os;
    }
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
