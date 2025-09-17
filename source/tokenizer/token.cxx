#include <app/limits.hxx>
#include <app/tokenizer/token.hxx>

#include <cassert>

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

    auto Token::char2kind( Char ch ) -> Kind {
        switch ( ch ) {
            case ':' : return Kind::ScopeBeg;
            case ';' : return Kind::ScopeEnd;
            case ',' : return Kind::Comma;
            default  : break;
        }
        return Kind::Unknown;
    }

    bool Token::is_single( Char ch ) {
        static Value singles{ ",:;" };
        return singles.find( ch ) != Value::npos;
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
