#include <app/compiler.hxx>

#include <fstream>

void app::Compiler::transfer( ) {
    std::ofstream output_file{ name_ };
    if ( !output_file.is_open( ) ) throw std::runtime_error( "Failed to open output file" );
    output_file << output_;
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
