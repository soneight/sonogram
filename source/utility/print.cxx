#include <app/alias.hxx>
#include <app/utility/print.hxx>

#include <array>
#include <iostream>
#include <string_view>

namespace app {

    auto to_string( Token::Kind kind ) -> std::string_view {
        using std::operator""sv;
        static constexpr std::array< std::string_view, static_cast< Size >( Token::Kind::Last ) > kinds{{
            "IdentifierDefault"sv,
            "IdentifierCaps"sv,
            "KeywordProgram"sv,
            "KeywordEcho"sv,
            "LiteralString"sv,
            "ScopeBeg"sv,
            "ScopeEnd"sv,
            "Comma"sv,
            "Unknown"sv
        }};
        return kinds[static_cast< Size >( kind )];
    }

    std::ostream &operator<<( std::ostream &os, Token const &token ) {
        os << "Token{ ";
        os << (token.has_kind( ) ? to_string( token.kind( ) ) : "Kindless") << ", ";
        os << (token.has_value( ) ? token.value( ) : "Valueless") << ", ";

        if ( token.has_position( ) ) {
            os << "line: " << token.line( ) << ", column: " << token.column( );
        } else {
            os << "Positionless";
        }
        os << " }";
        return os;
    }
}
