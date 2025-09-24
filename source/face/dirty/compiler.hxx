#ifndef DIRTY_COMPILER_HXX
#define DIRTY_COMPILER_HXX

#include <app/lexer.hxx>
#include <dirty/parser.hxx>

#include <string>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace dirty {
    using namespace app;
    class Compiler final {
        using String = std::string;
        String source_;
        String output_;
        String name_{ "a.cxx" };
    public:
        Compiler( String &&source ) : source_( std::move( source ) ) { }
        // TODO: from sonogram (*.son) to intermediate representation (*.sonir)
        void generate( ) {
            std::cout << "Generate started" << std::endl;
            std::cout << "Generation tokens" << std::endl;
            Lexer lexer{ source_ };
            std::cout << "Tokens count: " << std::distance( lexer.begin( ), lexer.end( ) ) << std::endl;
            std::cout << "Parsing tokens" << std::endl;
            Parser parser{ { lexer.begin( ), lexer.end( ) } };
            output_ = parser.str( );
            std::cout << "Generated file size: " << output_.size( ) << " bytes" << std::endl;
            name_ = parser.name( ) + ".cxx";
        }
        // from intermediate representation (*.sonir) to C++17 (*.cxx)
        void transfer( ) {
            std::cout << "Transferring C++ code into file: " << name_ << std::endl;
            std::ofstream output_file{ name_ };
            if ( !output_file.is_open( ) ) throw std::runtime_error( "Failed to open output file" );
            output_file << output_;
            output_file.close( );
            std::cout << "Transfer completed" << std::endl;
        }
    }; // class Compiler

} // namespace dirty

#endif//DIRTY_COMPILER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
