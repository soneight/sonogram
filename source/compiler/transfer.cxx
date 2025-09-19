#include <app/compiler.hxx>

#include <fstream>

void app::Compiler::transfer( ) {
    // TODO make only transfer to work with output files, as for IR and CXX both
    std::ofstream output_file{ name_ };
    if ( !output_file.is_open( ) ) throw std::runtime_error( "Failed to open output file" );
    output_file << output_;
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
