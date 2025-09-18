#ifndef APP_TOKENIZER_HXX
#define APP_TOKENIZER_HXX

#include <app/alias.hxx>
#include <app/tokenizer/token.hxx>
#include <app/utility/error.hxx>

#include <string>
#include <string_view>
#include <vector>

namespace app {

    class Tokenizer {
        using Tokens = std::vector< Token >;
        using Message = std::string;
        using Source = std::string_view;
        using Pos = Source::size_type;
        static inline constexpr auto EndPos = Source::npos;
        Source source_;
        Tokens tokens_{ };
        // current reading position
        Pos pos_{ 0 };
        // current line and column numbers
        unsigned line_ { 1 };
        unsigned column_ { 1 };
        Error error_{ };
        // advance to the next non-space character
        void nextchar( );
        // check if position is within source bounds
        bool in_source( Pos from ) const;
        void add_token( Token::Kind kind, Pos to );
        void add_error( Message message );
        void parse_single( );
        void parse_identifier( );
        void parse_string( );
        void parse_keyword( );
    public:
        // Important: source lifetime must be greater than Tokenizer lifetime
        explicit Tokenizer( Source source );
        auto begin( ) const { return tokens_.begin( ); }
        auto end( ) const { return tokens_.end( ); }
    }; // class Tokenizer

} // namespace app

#endif//APP_TOKENIZER_HXX
