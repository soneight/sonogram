#ifndef APP_TOKENIZER_TOKEN_HXX
#define APP_TOKENIZER_TOKEN_HXX

#ifndef SON8_SONOGRAM_INCLUDE_OLD_IMPLEMENTATION
#error "File is subject to be removed. Use source/face/app/lexer/token.hxx instead."
#endif

#include <app/alias.hxx>

#include <array>
#include <string_view>

namespace app {

    class Token {
        using Value_ = std::string_view;
        enum class Kind_ : Unt1 {
            IdentifierDefault,
            IdentifierCaps,
            LiteralString,
            // IMPORTANT: keywords must be contiguous
            KEYWORD_beg, // skip
            KeywordProgram = KEYWORD_beg,
            KeywordEcho,
            KEYWORD_end, // skip
            // IMPORTANT: single-character tokens must be contiguous
            SINGLE_beg = KEYWORD_end,
            ScopeBeg = SINGLE_beg,
            ScopeEnd,
            Comma,
            SINGLE_end, // skip
            Unknown = SINGLE_end,
            // IMPORTANT: keep this last, last is a true last element
            Last
        };
        // keywords helpers
        static inline constexpr Size Keyword_Beg_ = static_cast< Size >( Kind_::KEYWORD_beg );
        static inline constexpr Size Keyword_End_ = static_cast< Size >( Kind_::KEYWORD_end );
        static inline constexpr Size Keyword_Count_ = Keyword_End_ - Keyword_Beg_;
        using Keywords = std::array< Value_, Keyword_Count_ >;
        static constexpr Keywords keywords_{{
            "-program", "-echo"
        }};
        static_assert( keywords_.size( ) == Keyword_Count_ );
        // single-character token helpers
        static inline constexpr Size Single_Beg_ = static_cast< Size >( Kind_::SINGLE_beg );
        static inline constexpr Size Single_End_ = static_cast< Size >( Kind_::SINGLE_end );
        static inline constexpr Size Single_Count_ = Single_End_ - Single_Beg_;
        using Singles = std::array< Char, Single_Count_ >;
        static constexpr Singles singles_{{
            ':', ';', ','
        }};
        static_assert( singles_.size( ) == Single_Count_ );
        // data members
        Kind_ kind_{ Kind_::Last };
        Value_ value_{ };
        unsigned line_{ 0 };
        unsigned column_{ 0 };
    public:
        using Kind = Kind_;
        using Value = Value_;
        // constructors
        Token( ) = default;
        explicit Token( Kind kind );
        Token( Kind kind, Value value );
        Token( Kind kind, Value value, unsigned line, unsigned column );
        // getters
        [[nodiscard]] Kind kind( ) const;
        [[nodiscard]] auto value( ) const -> Value;
        [[nodiscard]] auto line( ) const -> unsigned;
        [[nodiscard]] auto column( ) const -> unsigned;
        // checkers
        [[nodiscard]] bool has_kind( ) const;
        [[nodiscard]] bool has_value( ) const;
        [[nodiscard]] bool has_position( ) const;
        // Static utility functions
        static bool is_single( Char ch );
        static bool is_identifier_edge( Char ch );
        static bool is_identifier( Char ch );
        static auto char2kind( Char ch ) -> Kind;
        static auto keyword2kind( Value keyword ) -> Kind;
    }; // class Token

} // namespace app

#endif//APP_TOKENIZER_TOKEN_HXX
