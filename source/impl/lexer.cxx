// own header
#include <app/lexer.hxx> // Lexer
// app headers
#include <app/alias.hxx> // Char, Size
#include <app/lexer/token.hxx> // Token
// std headers
#include <cctype> // std::isspace
#include <string> // std::to_string
#include <stdexcept> // std::runtime_error

namespace app {

    Lexer::Lexer( Source source ) : source_( source ) {
        if ( source_.back( ) != '\n' ) throw std::runtime_error( "Source must end with a new line" );
        tokens_.reserve( source.size( ) / 8 ); // rough estimate
        while ( nextchar( ), pos_ != EndPos_ ) {
            alias::Char ch = source_[pos_];
            if ( Token::is_single( ch ) ) {
                scan_single( );
            } else if ( Token::is_identifier_edge( ch ) ) {
                scan_identifier( );
            } else if (  ch == '"' ) {
                scan_string( );
            } else if ( ch == '-' ) {
                scan_keyword( );
            } else {
                add_error( "Unknown character: " + Error::Message{ source_.substr( pos_, 8 ) } );
                ++pos_;
                ++column_;
            }
        }
        error_.check_exception( );
    }

    void Lexer::add_token( Token::Kind kind, Pos_ to ) {
        alias::Size size = to - pos_;
        tokens_.emplace_back( kind, source_.substr( pos_, size ), line_, column_ );
        pos_ += size;
        column_ += size;
    }

    void Lexer::add_error( Error::Message message ) {
        error_.add( "\t" + message + ", Ln: " + std::to_string( line_ ) + ", Col: " + std::to_string( column_ ) );
    }

    bool Lexer::in_source( Pos_ from ) const {
        return from < source_.size( );
    }

    void Lexer::nextchar( ) {
        using std::operator""sv;
        auto nextline = [this]( ) { ++line_; column_ = 1; };
        for ( auto i = pos_; in_source( i ); ++i ) {
            alias::Char ch = source_[i];
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
        pos_ = EndPos_;
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
