// app headers
#include <app/compiler.hxx>
// fetch headers
#include <son8/main.hxx>
// std headers
#include <iostream>

void son8::main( Args const &args ) {
    std::cout << *args.begin( ) << std::endl;

    app::Compiler compiler;
    std::cout << "Info: generating intermediate representation..." << std::endl;
    compiler.generate( );
    std::cout << "Info: transferring to C++17..." << std::endl;
    compiler.transfer( );
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
