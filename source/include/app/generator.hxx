#ifndef APP_GENERATOR_HXX
#define APP_GENERATOR_HXX

#include <app/tokenizer.hxx>

#include <string>
#include <stdexcept>

namespace app {

    class Generator {
        enum class Context {
            Global,
            Statement,
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
            if ( pos_->kind( ) != Token::Kind::Comma && pos_->kind( ) != Token::Kind::ScopeEnd ) throw std::runtime_error( "Expected ',' or ';' after statement" );
            ++pos_;
        }
    public:
        Generator( Tokens const &tokens ) : pos_( tokens.begin( ) ), end_( tokens.end( ) ) {
            while ( pos_ != end_ ) {
                switch ( context_ ) {
                    case Context::Global: process_global( ); break;
                    case Context::Statement: process_statement( ); break;
                    default: throw std::runtime_error( "Unknown generator context" );
                }
            }
        }

        String str( ) {
            std::string result;
            if ( include_iostream_ ) result += "#include <iostream>\n\n";
            result += "int main( ) {\n";
            result += main_body_;
            result += "}\n";
            return result;
        }

        String name( ) {
            return program_name_;
        }
    };

} // namespace app

#endif//APP_GENERATOR_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
