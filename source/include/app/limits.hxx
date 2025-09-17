#ifndef APP_LIMITS_HXX
#define APP_LIMITS_HXX

#include <cassert>

namespace app::limits {
    // sorted from the smallest to the largest and then by alpyhabetical order
#if APP_LIMITS_FORCE_ERROR_COUNT_MAX > 0
    static inline constexpr unsigned Error_Count_Max       = APP_LIMITS_FORCE_ERROR_COUNT_MAX;
#else
    static inline constexpr unsigned Error_Count_Max       = 0b1 << 3; // 8 errors
#endif
    static inline constexpr unsigned Identifier_Length_Max = 0b1 << 5; // 32 bytes (characters)
    static inline constexpr unsigned File_Size_Max         = 0b1 << 16; // 64 KiB
    static inline constexpr unsigned Lines_Count_Max       = 0b1 << 10; // 1 KiB
    static inline constexpr unsigned Columns_Count_Max     = 0b1 << 8; // 256 bytes (characters)

    class Limit {
        unsigned value_{ 0 };
        unsigned limit_{ 0 };
    public:
        Limit( ) = default;
        explicit Limit( unsigned value, unsigned limit )
            : value_( value ), limit_( limit ) {
            assert( value_ <= limit_ );
        }

    };

} // namespace app::limits

#endif//APP_LIMITS_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
