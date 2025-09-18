#include <app/limits.hxx>
#include <app/tokenizer/token.hxx>

#include <algorithm>
#include <cassert>
#include <string>

namespace app {

    Token::Token( Kind kind ) : kind_( kind ) { assert( kind_ < Kind::Last ); }

    Token::Token( Kind kind, Value value ) : kind_( kind ), value_( value ) {
        assert( kind_ < Kind::Last );
        assert( !value_.empty( ) );
    }

    Token::Token( Kind kind, Value value, unsigned line, unsigned column )
        : kind_( kind ), value_( value ), line_( line ), column_( column ) {
        assert( kind_ < Kind::Last );
        assert( !value_.empty( ) );
        assert( line_ != 0 && line_ <= limits::Lines_Count_Max );
        assert( column_ != 0 && column_ <= limits::Columns_Count_Max );
    }

    auto Token::kind( ) const -> Kind {
        assert( kind_ < Kind::Last );
        return kind_;
    }

    auto Token::value( ) const -> Value {
        assert( !value_.empty( ) );
        return value_;
    }
    auto Token::line( ) const -> unsigned {
        assert( line_ != 0 && line_ <= limits::Lines_Count_Max );
        return line_;
    }

    auto Token::column( ) const -> unsigned {
        assert( column_ != 0 && column_ <= limits::Columns_Count_Max );
        return column_;
    }

    bool Token::has_kind( ) const { return kind_ < Kind::Last; }

    bool Token::has_value( ) const { return !value_.empty( ); }

    bool Token::has_position( ) const { return line_ != 0 && column_ != 0; }

    bool Token::is_single( Char ch ) {
        static std::string singles{ ",:;" };
        return singles.find( ch ) != std::string::npos;
    }

    bool Token::is_identifier_edge( Char ch ) {
        ch = std::tolower( ch );
        return ch >= 'a' && ch <= 'z';
    }

    bool Token::is_identifier( Char ch ) {
        return is_identifier_edge( ch ) || ch == '_';
    }

    auto Token::char2kind( Char ch ) -> Kind {
        auto it = std::find( singles_.begin( ), singles_.end( ), ch );
        if ( it == singles_.end( ) ) return Kind::Last;

        auto index = std::distance( singles_.begin( ), it );
        return static_cast< Kind >( static_cast< Size >( Kind::SINGLE_beg ) + index );
    }

    auto Token::keyword2kind( Value keyword ) -> Kind {
        auto it = std::find( keywords_.begin( ), keywords_.end( ), keyword );
        if ( it == keywords_.end( ) ) return Kind::Last;

        auto index = std::distance( keywords_.begin( ), it );
        return static_cast< Kind >( static_cast< Size >( Kind::KEYWORD_beg ) + index );
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
