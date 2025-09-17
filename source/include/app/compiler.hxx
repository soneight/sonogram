#ifndef SON8_SOURCE_APP_COMPILER_HXX
#define SON8_SOURCE_APP_COMPILER_HXX

namespace app {

    class Compiler {
    public:
        // from sonogram to intermediate representation
        void generate( );
        // from intermediate representation to C++17
        void transfer( );
    };

} // namespace app

#endif//SON8_SOURCE_APP_COMPILER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
