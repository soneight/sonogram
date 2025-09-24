#ifndef DIRTY_PARSER_HXX
#define DIRTY_PARSER_HXX

#include <app/lexer.hxx> // Tokens
#include <app/utility/error.hxx>
#include <app/utility/print.hxx>

#include <stdexcept>
#include <string>

namespace dirty {
    using namespace app;
    // TODO: for now dirty implementation for see how to better organize code later
    class Parser {
        enum class Context {
            Global,
            Statement,
            EndOfFile,
        };
        using String = std::string;
        using ItToken = Tokens::const_iterator;
        // data members
        String program_name_{ };
        String main_body_{ };
        bool include_iostream_{ false };
        ItToken pos_;
        ItToken end_;
        Error error_;
        Context context_{ Context::Global };
        // utility functions
        void add_error( String const &message ) {
            error_.add( message + ", but got " + ( pos_ != end_ ? app::to_string( *pos_ ) : "NOTHING!") );
        }
        // processing functions
        void process_global( ) {
            if ( pos_->kind( ) != Token::Kind::IdentifierDefault ) {
                add_error( "Expected program name identifier" );
                ++pos_;
                return;
            }
            program_name_ = std::string{ pos_->value( ) };
            if ( ++pos_ != end_ && pos_->kind( ) != Token::Kind::KeywordProgram ) {
                add_error( "Expected -program keyword" );
                ++pos_;
                return;
            }
            if ( ++pos_ != end_ && pos_->kind( ) != Token::Kind::ScopeBeg ) {
                add_error( "Expected ':'" );
                ++pos_;
                return;
            }
            ++pos_;
            context_ = Context::Statement;
        }
        void process_statement( ) {
            if ( pos_->kind( ) == Token::Kind::KeywordProgram ) add_error( "Nested -program is not allowed" );
            if ( pos_->kind( ) == Token::Kind::KeywordEcho ) {
                include_iostream_ = true;
                ++pos_;
                if ( pos_ != end_ && pos_->kind( ) != Token::Kind::LiteralString ) add_error( "Expected string literal after -echo" );
                main_body_ += "    std::cout << " + std::string{ pos_->value( ) } + ";\n";
                ++pos_;
                return;
            }
            if ( pos_->kind( ) == Token::Kind::Comma ) {
                ++pos_;
                return;
            }
            if ( pos_->kind( ) == Token::Kind::ScopeEnd ) {
                context_ = Context::EndOfFile;
                ++pos_;
                return;
            }
            add_error( "Unexpected" );
            ++pos_;
        }
        void process_end_of_file( ) {
            if ( pos_ != end_ ) add_error( "Expected end of file" );
            ++pos_;
        }
    public:
        Parser( Tokens const &tokens ) : pos_( tokens.begin( ) ), end_( tokens.end( ) ) {
            while ( pos_ != end_ ) {
                switch ( context_ ) {
                case Context::Global: process_global( ); break;
                case Context::Statement: process_statement( ); break;
                case Context::EndOfFile: process_end_of_file( ); break;
                    default: throw std::runtime_error( "Unknown parser context" );
                }
            }
            if ( context_ != Context::EndOfFile ) add_error( "Unexpected end of file" );
            error_.check_exception( );
        }

        auto str( ) -> String {
            std::string result;
            if ( include_iostream_ ) result += "#include <iostream>\n\n";
            result += "int main( ) {\n";
            result += main_body_;
            result += "}\n";
            return result;
        }

        auto name( ) -> String {
            return program_name_;
        }
    }; // class Parser

} // namespace dirty

#endif//DIRTY_PARSER_HXX
