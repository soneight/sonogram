#ifndef APP_UTILITY_ERROR_HXX
#define APP_UTILITY_ERROR_HXX

#include <app/limits.hxx>

#include <string>

namespace app {

    class Error final {
        using Str = std::string;
        Str messege_;
        unsigned limit_{ app::limits::Error_Count_Max  };
        void throw_exception( ) const;
    public:
        Error( ) = default;
        void add( Str const & messege );
        bool has_errors( ) const;
        void check_exception( ) const;
    }; // class Error

} // namespace app

#endif//APP_UTILITY_ERROR_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
