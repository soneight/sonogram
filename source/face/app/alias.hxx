#ifndef APP_ALIAS_HXX
#define APP_ALIAS_HXX

#include <cstddef> // std::size_t
#include <cstdint> // std::u?int*_t

namespace app::alias {
    // Remark, use std types for defining own types instead of aliases themselves
    //    for better tools support (e.g. on hovering)
    using Char = unsigned char;
    using Size = std::size_t;

    using Int0 = std::int8_t;
    using Int1 = std::int16_t;
    using Int2 = std::int32_t;
    using Int3 = std::int64_t;

    using Unt0 = std::uint8_t;
    using Unt1 = std::uint16_t;
    using Unt2 = std::uint32_t;
    using Unt3 = std::uint64_t;

    using Column = std::uint16_t;
    using Line = std::uint32_t;

} // namespace app

#endif//APP_ALIAS_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
