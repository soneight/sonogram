// app headers
#include <app.hxx>
#include <dirty/parser.hxx>
#include <dirty/compiler.hxx>
// son headers
#include <son8/main.hxx>
// std headers
#include <iostream>
#include <fstream>
#include <stdexcept>

void son8::main( Args const &args ) try {
    // print first argument (program name)
    std::cout << *args.begin( ) << std::endl;

    if ( args.size( ) != 2 ) throw std::runtime_error( "Expected exactly one argument - source file path" );
    auto filename = std::string{ *(args.begin( ) + 1) };
    std::ifstream source_file{ filename };
    if ( !source_file.is_open( ) ) throw std::runtime_error( "Failed to open source file: " + filename );
    using ItStreamBuf = std::istreambuf_iterator<char>;
    std::string source{ ItStreamBuf(source_file), ItStreamBuf( ) };
    source_file.close( );

    dirty::Compiler compiler{ std::move(source) };
    std::cout << "Info: generating intermediate representation..." << std::endl;
    compiler.generate( );
    std::cout << "Info: transferring to C++17..." << std::endl;
    compiler.transfer( );
    exit( EXIT_SUCCESS );
} catch ( std::exception const &e ) {
    std::cerr << "Error: " << e.what( ) << std::endl;
    exit( EXIT_FAILURE );
} catch ( ... ) {
    std::cerr << "Error: unknown exception caught" << std::endl;
    exit( EXIT_FAILURE );
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
