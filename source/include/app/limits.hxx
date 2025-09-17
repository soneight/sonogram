#ifndef APP_LIMITS_HXX
#define APP_LIMITS_HXX

namespace app::limits {

    static inline constexpr int const File_Size_Max = 0b1 << 16; // 64 KiB
    static inline constexpr int const Token_Length_Max = 0b1 << 5; // 32 bytes (characters)

} // namespace app::limits

#endif//APP_LIMITS_HXX
