#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>


namespace Mem64
{

struct NativeHandle
{
    using addr_t = std::uintptr_t;
    using saddr_t = std::intptr_t;
    using usize_t = std::size_t;
    using ssize_t = std::ssize_t;

    static constexpr addr_t INVALID_OFFSET{static_cast<addr_t>(nullptr)};

    /// Read n bytes from offset
    void read_raw(addr_t offset, std::uint8_t data[], usize_t n)
    {
        std::copy_n(reinterpret_cast<std::uint8_t*>(offset), n, data);
    }

    /// Write n bytes to offset
    void write_raw(addr_t offset, const std::uint8_t data[], usize_t n)
    {
        std::copy_n(data, n, reinterpret_cast<std::uint8_t*>(offset));
    }

    /// Read T from offset
    template<typename T>
    T read(addr_t offset)
    {
        static_assert(std::is_fundamental_v(T));
        return *reinterpret_cast<const T*>(offset);
    }

    /// Write T to offset
    template<typename T>
    void write(addr_t offset, T val)
    {
        static_assert(std::is_fundamental_v(T));
        *reinterpret_cast<T*>(offset) = val;
    }

    template<typename T>
    static bool valid_offset(addr_t offset)
    {
        return (offset != INVALID_OFFSET) && (offset % alignof(T) == 0);
    }
};

} // Mem64

