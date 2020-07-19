#pragma once

#include <type_traits>
#include "util.hpp"


namespace Mem64
{

template<typename TType, typename THandle>
struct Ptr
{
    using Traits = PtrTraits<TType, THandle>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using SSizeType = typename Traits::SSizeType;
    using USizeType = typename Traits::USizeType;
    using QualifiedType = typename Traits::QualifiedType;

    static constexpr USizeType SIZE{Traits::SIZE};

    Ptr():
        mem_hdl_(),
        addr_(HandleType::INVALID_OFFSET)
    {}

    explicit Ptr(const HandleType& hdl):
        mem_hdl_{hdl}, addr_{HandleType::INVALID_OFFSET}
    {}

    Ptr(const HandleType& hdl, AddrType addr):
        mem_hdl_{hdl}, addr_{addr}
    {}

    void set_hdl(std::optional<HandleType> hdl)
    {
        mem_hdl_ = std::move(hdl);
    }

    void set_offset(AddrType addr)
    {
        addr_ = addr;
    }

    void invalidate()
    {
        mem_hdl_ = {};
        addr_ = HandleType::INVALID_OFFSET;
    }

    std::optional<HandleType> hdl() const
    {
        return mem_hdl_;
    }

    AddrType offset() const
    {
        return addr_;
    }

    bool valid() const
    {
        return HandleType::template valid_offset<QualifiedType>(addr_) && mem_hdl_.has_value();
    }

    Ref<QualifiedType, HandleType> operator*() const
    {
        return {*mem_hdl_, addr_};
    }

    Ref<QualifiedType, HandleType> operator[](USizeType i) const
    {
        return {*mem_hdl_, addr_ + SIZE * i};
    }

    OperatorProxy<Ref<QualifiedType, HandleType>>
    operator->() const
    {
        return Ref<QualifiedType, HandleType>{*mem_hdl_, addr_};
    }

    #define IF_CONVERTIBLE_ template<typename T, std::enable_if_t<std::is_convertible_v<QualifiedType*, T>>>

    IF_CONVERTIBLE_
    explicit operator Ptr<T, HandleType>() const
    {
        return {mem_hdl_, addr_};
    }

    IF_CONVERTIBLE_
    bool operator==(const Ptr<T, HandleType>& other) const
    {
        return (mem_hdl_ == other.mem_hdl_ && addr_ == other.addr_);
    }

    IF_CONVERTIBLE_
    bool operator!=(const Ptr<T, HandleType>& other) const
    {
        return !(*this == other);
    }


    Ptr operator+(SSizeType n) const
    {
        return {mem_hdl_, addr_ + SIZE * n};
    }

    Ptr operator-(SSizeType n) const
    {
        return {mem_hdl_, addr_ - SIZE * n};
    }

    Ptr operator+=(SSizeType n)
    {
        addr_ += SIZE * n;
        return *this;
    }

    Ptr operator-=(SSizeType n)
    {
        addr_ -= SIZE * n;
        return *this;
    }

    Ptr& operator++()
    {
        addr_ += SIZE;
        return *this;
    }

    const Ptr operator++(int)
    {
        auto old{*this};
        *this++;
        return old;
    }

    Ptr& operator--()
    {
        addr_ -= SIZE;
        return *this;
    }

    const Ptr operator--(int)
    {
        auto old{*this};
        *this--;
        return old;
    }

    #undef IF_CONVERTIBLE_

private:
    std::optional<HandleType> mem_hdl_;
    AddrType addr_;
};

} // Mem64
