#ifndef APP_LIMITS_HXX
#define APP_LIMITS_HXX

namespace app::limits {
    // sorted from the smallest to the largest and then by alpyhabetical order
    static inline constexpr int Error_Count_Max       = 0b1 << 3; // 8 errors
    static_assert( Error_Count_Max > 0 );
    static inline constexpr int Identifier_Length_Max = 0b1 << 5; // 32 bytes (characters)
    static inline constexpr int File_Size_Max         = 0b1 << 16; // 64 KiB

} // namespace app::limits

#endif//APP_LIMITS_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
