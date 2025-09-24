// own header
#include <app/lexer.hxx> // Lexer
// app headers
#include <app/lexer/token.hxx> // Token
// std headers

namespace app {

    void Lexer::scan_single( ) {
        auto kind = Token::char2kind( source_[pos_] );
        if ( kind == Token::Kind::Unknown ) {
            add_error( "Unknown single-character token" );
            ++pos_;
            ++column_;
            return;
        }
        add_token( kind, pos_ + 1 );
    }

    void Lexer::scan_string( ) {
        Pos_ to = pos_ + 1;
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

    void Lexer::scan_keyword( ) {
        Pos_ to = pos_ + 1;
        while ( in_source( to ) && Token::is_identifier_edge( source_[to] ) ) ++to;
        Source keyword{ source_.substr( pos_, to - pos_ ) };

        auto kind = Token::keyword2kind( keyword );
        if ( kind == Token::Kind::Unknown ) {
            add_error( "Unknown keyword: " + Error::Message{ keyword } );
            pos_ = to;
            return;
        }
        add_token( kind, to );
    }

    void Lexer::scan_identifier( ) {
        Pos_ to = pos_ + 1;
        while ( in_source( to ) && Token::is_identifier( source_[to] ) ) ++to;
        if ( source_[to - 1] == '_' ) {
            column_ += to - pos_ - 1;
            add_error( "Symbol '_' can appear only inside identifiers (cannot be leading or trailing)" );
            pos_ = to;
            return;
        }
        add_token( Token::Kind::IdentifierDefault, to );
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
