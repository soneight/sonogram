#ifndef APP_LIMITS_HXX
#define APP_LIMITS_HXX

#include <app/alias.hxx> // Size, Int0, Column, Line

namespace app::limits {

    inline constexpr alias::Int0 File_Limit_Shift = -3;
    static_assert( -3 <= File_Limit_Shift && File_Limit_Shift <= 5, "File_Limit_Shift must be in range [-3;5]" );

    class Max final {
        static inline constexpr alias::Int0 fls_ = File_Limit_Shift;
    public:
        static inline constexpr alias::Unt0 Error_Count       = 0b1u << 3u;  // 8 errors
        static inline constexpr alias::Size Identifier_Length = 0b1u << 4u;  // 16 bytes
        // file related limits
        static inline constexpr alias::Column Columns_Count   = 0b1u << ( 8u + ( fls_ < 0 ? fls_ : 0 ) );  // 256 bytes
        static inline constexpr alias::Line Lines_Count       = 0b1u << ( 13u + fls_ ); // 8 KiB
        // in source file columns usually not full, so dividing them by 4 (>> 2) for average columns per line estimation
        static inline constexpr alias::Size File_Size         = ( Columns_Count >> 2u ) * Lines_Count; // 512 KiB
    }; // class Max
}

#endif//APP_LIMITS_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
