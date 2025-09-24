#ifndef APP_LEXER_HXX
#define APP_LEXER_HXX
// app headers
#include <app/alias.hxx> // Line, Column
#include <app/lexer/token.hxx> // Token
#include <app/utility/error.hxx> // Error
// std headers
#include <string_view> // std::string_view
#include <vector> // std::vector

namespace app {

    using Tokens = std::vector< Token >;

    class Lexer final {
        using Source_ = std::string_view;
        using Pos_ = Source_::size_type;
        static inline constexpr auto EndPos_ = Source_::npos;
        Source_ source_;
        Tokens tokens_{ };
        // current reading position
        Pos_ pos_{ 0 };
        // current line and column numbers
        alias::Line line_ { 1 };
        alias::Column column_ { 1 };
        // error handling
        Error error_{ };
        // advance to the next non-space character
        void nextchar( );
        // check if position is within source bounds
        bool in_source( Pos_ from ) const;
        void add_token( Token::Kind kind, Pos_ to );
        void add_error( Error::Message message );
        // scanning functions
        void scan_single( );
        void scan_identifier( );
        void scan_string( );
        void scan_keyword( );
    public:
        using Source = Source_;
        // Important: source lifetime must outlive Lexer lifetime
        explicit Lexer( Source source );
        auto begin( ) const { return tokens_.begin( ); }
        auto end( ) const { return tokens_.end( ); }
    }; // class Lexer

} // namespace app

#endif//APP_LEXER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
