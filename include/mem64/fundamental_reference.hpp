#pragma once

#include "reference_common.hpp"


namespace Mem64
{

/// Reference to fundamental types
template<typename TType, typename THandle>
struct Ref<TType, THandle,
    std::enable_if_t<std::is_fundamental_v<TType> || std::is_enum_v<TType>>>
    : RefBase<Ref<TType, THandle>>
{
    using Traits = RefTraits<Ref<TType, THandle>>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using USizeType = typename Traits::USizeType;
    using RawType = typename Traits::RawType;
    using QualifiedType = typename Traits::QualifiedType;

    explicit Ref(const HandleType& hdl, AddrType addr = HandleType::INVALID_OFFSET):
    RefBase<Ref<TType, THandle>>(hdl, addr)
    {}

    operator RawType() const
    {
        return this->read();
    }

    void write(const RawType& val) const
    {
        this->mem_hdl_.template write<RawType>(this->addr_, val);
    }

    RawType read() const
    {
        return this->mem_hdl_.template read<RawType>(this->addr_);
    }

    #define MUTABLE_ONLY_ template<typename = std::enable_if_t<!RefTraits<Ref<QualifiedType, HandleType>>::IS_CONST>>

    MUTABLE_ONLY_
    RawType operator=(const RawType& other) const
    {
        write(other);
        return other;
    }

    RawType operator+(const RawType& other) const
    {
        return read() + other;
    }

    RawType operator-(const RawType& other) const
    {
        return read() - other;
    }

    RawType operator*(const RawType& other) const
    {
        return read() * other;
    }

    RawType operator/(const RawType& other) const
    {
        return read() / other;
    }

    RawType operator%(const RawType& other) const
    {
        return read() % other;
    }

    RawType operator&(const RawType& other) const
    {
        return read() & other;
    }

    RawType operator|(const RawType& other) const
    {
        return read() | other;
    }

    RawType operator^(const RawType& other) const
    {
        return read() ^ other;
    }

    MUTABLE_ONLY_
    RawType operator+=(const RawType& other) const
    {
        RawType val{read() + other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator-=(const RawType& other) const
    {
        RawType val{read() - other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator*=(const RawType& other) const
    {
        RawType val{read() * other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator/=(const RawType& other) const
    {
        RawType val{read() / other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator%=(const RawType& other) const
    {
        RawType val{read() % other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator&=(const RawType& other) const
    {
        RawType val{read() & other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator|=(const RawType& other) const
    {
        RawType val{read() | other};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator^=(const RawType& other) const
    {
        RawType val{read() ^ other};
        write(val);
        return val;
    }

    RawType operator~() const
    {
        return ~read();
    }

    RawType operator<<(std::size_t pos) const
    {
        return (read() << pos);
    }

    RawType operator>>(std::size_t pos) const
    {
        return (read() >> pos);
    }


    RawType operator<<=(std::size_t pos) const
    {
        RawType val{read() << pos};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator>>=(std::size_t pos) const
    {
        RawType val{read() >> pos};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator++() const
    {
        RawType val{read() + (RawType)1};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator++(int) const
    {
        RawType old_val{read()},
                new_val{old_val + (RawType)1};
        write(new_val);
        return old_val;
    }

    MUTABLE_ONLY_
    RawType operator--() const
    {
        RawType val{read() - (RawType)1};
        write(val);
        return val;
    }

    MUTABLE_ONLY_
    RawType operator--(int) const
    {
        RawType old_val{read()},
                new_val{old_val - (RawType)1};
        write(new_val);
        return old_val;
    }

    /// Forward comparison operators
    bool operator==(const RawType& other) const
    {
        return read() == other;
    }

    bool operator!=(const RawType& other) const
    {
        return !(*this == other);
    }

    bool operator<(const RawType& other) const
    {
        return read() < other;
    }

    bool operator<=(const RawType& other) const
    {
        return !(*this > other);
    }

    bool operator>(const RawType& other) const
    {
        return read() > other;
    }

    bool operator>=(const RawType& other) const
    {
        return !(*this < other);
    }

    #undef MUTABLE_ONLY_
};

} // Mem64

