#include <app/alias.hxx>
#include <app/utility/print.hxx>

#include <array>
#include <cassert>
#include <iostream>
#include <string_view>

namespace app {

    auto to_string( Token::Kind kind ) -> std::string {
        using std::operator""sv;
        static constexpr std::array< Token::Value, static_cast< Size >( Token::Kind::Last ) > kinds{{
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
        return std::string{ kinds[static_cast< Size >( kind )] };
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
