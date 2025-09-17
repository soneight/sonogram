#include <son8/main.hxx>

#include <iostream>

void son8::main( Args const &args ) {
    std::cout << *args.begin( ) << std::endl;
}
