#include <app/tokenizer.hxx>

#include <array>
#include <algorithm>
#include <cctype>
#include <string>
#include <stdexcept>

namespace app {

    namespace detail {
        bool is_single( Char ch ) {
            static std::string singles{ ",:;" };
            return singles.find( ch ) != std::string::npos;
        }

        bool is_identifier_edge( Char ch ) {
            ch = std::tolower( ch );
            return ch >= 'a' && ch <= 'z';
        }

        bool is_identifier( Char ch ) {
            return is_identifier_edge( ch ) || ch == '_';
        }
    }

    Tokenizer::Tokenizer( Source source ) : source_( source ) {
        if ( source_.back( ) != '\n' ) throw std::runtime_error( "Source must end with a new line" );
        tokens_.reserve( source.size( ) / 8 ); // rough estimate
        while ( nextchar( ), pos_ != EndPos ) {
            Char ch = source_[pos_];
            if ( detail::is_single( ch ) ) {
                parse_single( );
            } else if ( detail::is_identifier_edge( ch ) ) {
                parse_identifier( );
            } else if (  ch == '"' ) {
                parse_string( );
            } else if ( ch == '-' ) {
                parse_keyword( );
            } else {
                add_error( "Unknown character (cast to int): " + Message{ std::to_string(static_cast<unsigned>(ch)) } );
                ++pos_;
                ++column_;
            }
        }
        error_.check_exception( );
    }

    void Tokenizer::parse_single( ) {
        auto kind = Token::char2kind( source_[pos_] );
        add_token( kind, pos_ + 1 );
    }

    void Tokenizer::parse_string( ) {
        Pos to = pos_ + 1;
        while ( in_source( to ) && source_[to] != '"' ) {
            if ( source_[to] == '\n' ) {
                add_error( "New line before closing string literal" );
                pos_ = to;
                return;
            }
            ++to;
        }
        add_token( Token::Kind::LiteralString, to + 1 );
    }

    void Tokenizer::parse_keyword( ) {
        using std::operator""sv;
        using Keywords = std::array< std::string_view, 2 >;
        static constexpr Keywords keywords{{
            "-program"sv, "-echo"sv
        }};
        using Kinds = std::array< Token::Kind, 2 >;
        static constexpr Kinds kinds{{
            Token::Kind::KeywordProgram, Token::Kind::KeywordEcho
        }};
        Pos to = pos_ + 1;
        while ( in_source( to ) && detail::is_identifier_edge( source_[to] ) ) ++to;
        Source keyword{ source_.substr( pos_, to - pos_ ) };
        auto it = std::find( keywords.begin( ), keywords.end( ), keyword );
        if ( it == keywords.end( ) ) {
            add_error( "Unknown keyword: " + Message{ keyword } );
            pos_ = to;
            return;
        }
        add_token( kinds[std::distance(keywords.begin(), it)], to );
    }

    void Tokenizer::parse_identifier( ) {
        Pos to = pos_ + 1;
        while ( in_source( to ) && detail::is_identifier( source_[to] ) ) ++to;
        add_token( Token::Kind::IdentifierDefault, to );
    }

    void Tokenizer::add_token( Token::Kind kind, Pos to ) {
        Size size = to - pos_;
        tokens_.emplace_back( kind, source_.substr( pos_, size), line_, column_ );
        pos_ += size;
        column_ += size;
    }

    void Tokenizer::add_error( Message message ) {
        error_.add( message + ", Ln: " + std::to_string( line_ ) + ", Col: " + std::to_string( column_ ) );
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
