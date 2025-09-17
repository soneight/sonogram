#ifndef APP_TOKENIZER_TOKEN_HXX
#define APP_TOKENIZER_TOKEN_HXX

#include <string_view>

namespace app {

    class Token {
        using Value = std::string_view;
        using Char = unsigned char;
        enum class Kind_ {
            IdentifierDefault,
            IdentifierCaps,
            KeywordProgram,
            KeywordEcho,
            LiteralString,
            ScopeBeg,
            ScopeEnd,
            Comma,
            Unknown,
            // Important: keep this last
            Last
        };
        Kind_ kind_{ Kind_::Last };
        Value value_{ };
        unsigned line_{ 0 };
        unsigned column_{ 0 };
    public:
        using Kind = Kind_;
        Token( ) = default;
        explicit Token( Kind kind );
        Token( Kind kind, Value value );
        Token( Kind kind, Value value, unsigned line, unsigned column );
        [[nodiscard]] Kind kind( ) const;
        [[nodiscard]] auto value( ) const -> Value;
        [[nodiscard]] auto line( ) const -> unsigned;
        [[nodiscard]] auto column( ) const -> unsigned;

        static auto char2kind( Char ch ) -> Kind;
        static bool is_single( Char ch );
    }; // class Token

} // namespace app

#endif//APP_TOKENIZER_TOKEN_HXX
