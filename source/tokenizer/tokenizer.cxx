#include <app/tokenizer.hxx>

#include <cctype>

namespace app {

    Tokenizer::Tokenizer( Source source ) : source_( source ) {
        tokens_.reserve( source.size( ) / 8 ); // rough estimate
        while ( nextchar( ), pos_ != EndPos ) {
            Char ch = source_[pos_];
            if ( Token::is_single( ch ) ) {
                parse_single( ch );

            } else {
                ++pos_;
            }
        }
    }

    void Tokenizer::parse_single( Char ch ) {
        auto kind = Token::char2kind( ch );
        add_token( kind, pos_ + 1 );
    }

    void Tokenizer::add_token( Token::Kind kind, Pos to ) {
        Size size = to - pos_;
        tokens_.emplace_back( kind, source_.substr( pos_, size), line_, column_ );
        pos_ += size;
        column_ += size;
    }

    bool Tokenizer::in_source( Pos from ) const {
        return from < source_.size( );
    }

    void Tokenizer::nextchar( ) {
        using std::operator""sv;
        auto nextline = [this]( ) { ++line_; column_ = 1; };
        for ( auto i = pos_; in_source( i ); ++i ) {
            Char ch = source_[i];
            if ( std::isspace( ch ) ) {
                if ( ch == '\n' ) nextline( );
                else ++column_;
            } else if ( ch == '#' || !source_.compare( i, 3, "\xE2\x92\xB8"sv ) ) { // comment start with `#` or `Ⓒ`
                nextline( );
                i = source_.find( '\n', i );
            } else {
                pos_ = i;
                return;
            }
        }
        pos_ = EndPos;
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
