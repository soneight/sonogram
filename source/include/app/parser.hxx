#ifndef APP_PARSER_HXX
#define APP_PARSER_HXX

#include <app/tokenizer.hxx>

#include <stdexcept>
#include <string>

namespace app {
    // TODO: for now dirty implementation for see how to better organize code later
    class Parser {
        enum class Context {
            Global,
            Statement,
            EndOfFile,
        };
        using String = std::string;
        String program_name_{ };
        String main_body_{ };
        bool include_iostream_{ false };
        Tokens::const_iterator pos_;
        Tokens::const_iterator end_;
        Context context_{ Context::Global };
        void process_global( ) {
            if ( pos_->kind( ) != Token::Kind::IdentifierDefault ) throw std::runtime_error( "Expected program name identifier" );
            program_name_ = std::string{ pos_->value( ) };
            ++pos_;
            if ( pos_->kind( ) != Token::Kind::KeywordProgram ) throw std::runtime_error( "Expected -program keyword" );
            ++pos_;
            if ( pos_->kind( ) != Token::Kind::ScopeBeg ) throw std::runtime_error( "Expected ':' token" );
            ++pos_;
            context_ = Context::Statement;
        }
        void process_statement( ) {
            if ( pos_->kind( ) == Token::Kind::KeywordProgram ) throw std::runtime_error( "Nested -program is not allowed" );
            if ( pos_->kind( ) == Token::Kind::KeywordEcho ) {
                include_iostream_ = true;
                ++pos_;
                if ( pos_->kind( ) != Token::Kind::LiteralString ) throw std::runtime_error( "Expected string literal after -echo" );
                main_body_ += "    std::cout << " + std::string{ pos_->value( ) } + ";\n";
                ++pos_;
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
            throw std::runtime_error( "Unexpected token" );
            ++pos_;
        }
        void process_end_of_file( ) {
            if ( pos_ != end_ ) throw std::runtime_error( "Expected end of file" );
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
            if ( context_ != Context::EndOfFile ) throw std::runtime_error( "Unexpected end of file" );
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
}

#endif//APP_PARSER_HXX
