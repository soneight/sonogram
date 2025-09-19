#ifndef APP_COMPILER_HXX
#define APP_COMPILER_HXX

#include <string>
#include <utility>

namespace app {

    class Compiler {
        using String = std::string;
        String source_;
        String output_;
        String name_{ "a.cxx" };
    public:
        Compiler( String &&source );
        // from sonogram to intermediate representation
        void generate( );
        // from intermediate representation to C++17
        void transfer( );
    }; // class Compiler

} // namespace app

#endif//APP_COMPILER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
