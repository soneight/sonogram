#include <app/compiler.hxx>

#include <app/tokenizer.hxx>
#include <app/generator.hxx>

void app::Compiler::generate( ) {
    // TODO: implement generation from sonogram to intermediate representation
    Tokenizer tokenizer{ source_ };
    Generator generator{ { tokenizer.begin( ), tokenizer.end( ) } };
    output_ = generator.str( );
    name_ = generator.name( ) + ".cxx";
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
