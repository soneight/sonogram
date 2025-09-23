#ifndef APP_UTILITY_ERROR_HXX
#define APP_UTILITY_ERROR_HXX
// app headers
#include <app/limits.hxx>
// std headers
#include <string>

namespace app {

    class Error final {
        static_assert( app::limits::Max::Error_Count > 0, "Error_Count must be positive" );
        using Message_ = std::string;
        Message_ message_;
        unsigned limit_{ app::limits::Max::Error_Count };
        void throw_exception( ) const;
    public:
        using Message = Message_;
        Error( ) = default;
        // modifying methods
        void add( Message const &message );
        void reset( );
        // const methods
        bool has_errors( ) const;
        void check_exception( ) const;

    }; // class Error

} // namespace app

#endif//APP_UTILITY_ERROR_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
