#include <app/compiler.hxx>

#include <app/tokenizer.hxx>
#include <app/parser.hxx>

void app::Compiler::generate( ) {
    // TODO: implement generation from sonogram to intermediate representation
    Tokenizer tokenizer{ source_ };
    Parser parser{ { tokenizer.begin( ), tokenizer.end( ) } };
    output_ = parser.str( );
    name_ = parser.name( ) + ".cxx";
}

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
