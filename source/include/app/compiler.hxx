#ifndef APP_COMPILER_HXX
#define APP_COMPILER_HXX

namespace app {

    class Compiler {
    public:
        // from sonogram to intermediate representation
        void generate( );
        // from intermediate representation to C++17
        void transfer( );
    }; // class Compiler

} // namespace app

#endif//APP_COMPILER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
