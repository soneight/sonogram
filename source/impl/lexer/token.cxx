// own header
#include <app/lexer/token.hxx> // class Token
// app headers
#include <app/limits.hxx> // limits::Max
// std headers
#include <algorithm> // std::find
#include <cassert> // assert
#include <iterator> // std::distance

namespace app {

    Token::Token( Kind kind ) : kind_( kind ) { assert( kind_ < Kind::Last ); }

    Token::Token( Kind kind, Value value ) : kind_( kind ), value_( value ) {
        assert( kind_ < Kind::Last );
        assert( !value_.empty( ) );
    }

    Token::Token( Kind kind, Value value, alias::Line line, alias::Column column )
        : kind_( kind ), value_( value ), line_( line ), column_( column ) {
        assert( kind_ < Kind::Last );
        assert( !value_.empty( ) );
        assert( line_ != 0 && line_ <= limits::Max::Lines_Count );
        assert( column_ != 0 && column_ <= limits::Max::Columns_Count );
    }

    auto Token::kind( ) const -> Kind {
        assert( kind_ < Kind::Last );
        return kind_;
    }

    auto Token::value( ) const -> Value {
        assert( !value_.empty( ) );
        return value_;
    }
    auto Token::line( ) const -> alias::Line {
        assert( line_ != 0 && line_ <= limits::Max::Lines_Count );
        return line_;
    }

    auto Token::column( ) const -> alias::Column {
        assert( column_ != 0 && column_ <= limits::Max::Columns_Count );
        return column_;
    }

    bool Token::has_kind( ) const { return kind_ < Kind::Last; }

    bool Token::has_value( ) const { return !value_.empty( ); }

    bool Token::has_position( ) const { return line_ != 0 && column_ != 0; }

    bool Token::is_single( alias::Char ch ) {
        return std::find( singles_.begin( ), singles_.end( ), ch ) != singles_.end( );
    }

    bool Token::is_identifier_edge( alias::Char ch ) {
        return ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' );
    }

    bool Token::is_identifier( alias::Char ch ) {
        return is_identifier_edge( ch ) || ch == '_';
    }

    auto Token::char2kind( alias::Char ch ) -> Kind {
        auto it = std::find( singles_.begin( ), singles_.end( ), ch );
        if ( it == singles_.end( ) ) return Kind::Unknown;

        auto index = std::distance( singles_.begin( ), it );
        return static_cast< Kind >( static_cast< alias::Size >( Kind::SINGLE_beg ) + index );
    }

    auto Token::keyword2kind( Value keyword ) -> Kind {
        auto it = std::find( keywords_.begin( ), keywords_.end( ), keyword );
        if ( it == keywords_.end( ) ) return Kind::Unknown;

        auto index = std::distance( keywords_.begin( ), it );
        return static_cast< Kind >( static_cast< alias::Size >( Kind::KEYWORD_beg ) + index );
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
