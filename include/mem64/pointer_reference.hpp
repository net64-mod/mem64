#pragma once

#include "reference_common.hpp"


namespace Mem64
{

/// Reference to pointer
template<typename TType, typename THandle>
struct Ref<TType, THandle, std::enable_if_t<is_nested_ptr_v<TType>>>
{
    using Traits = PtrTraits<TType, THandle>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using SSizeType = typename Traits::SSizeType;
    using USizeType = typename Traits::USizeType;
    using RawType = typename Traits::RawType;
    using QualifiedType = typename Traits::QualifiedType;

    using PtrType = Ptr<remove_nested_ptr_t<QualifiedType>, HandleType>;

    static constexpr USizeType SIZE{Traits::SIZE};


    Ref(const HandleType& hdl, AddrType addr):
    mem_hdl_{hdl}, addr_{addr}
    {}

    Ref& operator=(const Ref& other)
    {
        mem_hdl_ = other.mem_hdl_;
        write(other.offset());

        return *this;
    }

    Ref& operator=(const PtrType& other)
    {
        mem_hdl_ = other.mem_hdl_;
        write(other.offset());

        return *this;
    }

    operator PtrType() const
    {
        return {mem_hdl_, read()};
    }

    Ptr<QualifiedType, HandleType> ptr() const
    {
        return {mem_hdl_, addr_};
    }

    void set_hdl(const HandleType& hdl)
    {
        mem_hdl_ = hdl;
    }

    HandleType hdl() const
    {
        return mem_hdl_;
    }

    void set_offset(AddrType addr) const
    {
        write(addr);
    }

    void invalidate() const
    {
        write(HandleType::INVALID_OFFSET);
    }

    AddrType offset() const
    {
        return read();
    }

    bool valid() const
    {
        return HandleType::valid_offset(read());
    }

    Ref<remove_nested_ptr_t<QualifiedType>, HandleType> operator*() const
    {
        return {mem_hdl_, read()};
    }

    Ref<remove_nested_ptr_t<QualifiedType>, HandleType> operator[](USizeType index) const
    {
        return Ref<remove_nested_ptr_t<QualifiedType>, HandleType>{mem_hdl_, read() + index * SIZE};
    }

    OperatorProxy<Ref<remove_nested_ptr_t<QualifiedType>, HandleType>>
    operator->() const
    {
        return Ref<QualifiedType, HandleType>{mem_hdl_, read()};
    }

    #define IF_CONVERTIBLE_ template<typename T, typename = \
                            std::enable_if_t<std::is_convertible_v<remove_nested_ptr_t<QualifiedType>, T>>>
    #define MUTABLE_ONLY_ template<typename = std::enable_if_t<!std::is_const_v<QualifiedType>>>

    IF_CONVERTIBLE_
    explicit operator Ptr<T, HandleType>() const
    {
        return {mem_hdl_, read()};
    }

    IF_CONVERTIBLE_
    bool operator==(const Ref<T, HandleType>& other) const
    {
        return (mem_hdl_ == other.mem_hdl_ && read() == other.read());
    }

    IF_CONVERTIBLE_
    bool operator==(const Ptr<T, HandleType>& other) const
    {
        return (mem_hdl_ == other.mem_hdl_ && read() == other.addr_);
    }

    IF_CONVERTIBLE_
    bool operator!=(const Ref<T, HandleType>& other) const
    {
        return !(*this == other);
    }

    IF_CONVERTIBLE_
    bool operator!=(const Ptr<T, HandleType>& other) const
    {
        return !(*this == other);
    }


    PtrType operator+(SSizeType n) const
    {
        return {mem_hdl_, read() + SIZE * n};
    }

    PtrType operator-(SSizeType n) const
    {
        return {mem_hdl_, read() - SIZE * n};
    }

    MUTABLE_ONLY_
    PtrType operator+=(SSizeType n) const
    {
        write(read() + SIZE * n);
        return static_cast<Ptr<QualifiedType, HandleType>>(*this);
    }

    MUTABLE_ONLY_
    PtrType operator-=(SSizeType n) const
    {
        write(read() - SIZE * n);
        return static_cast<PtrType>(*this);
    }

    MUTABLE_ONLY_
    const PtrType operator++() const
    {
        write(read() + SIZE);
        return static_cast<PtrType>(*this);
    }

    MUTABLE_ONLY_
    const PtrType operator++(int) const
    {
        auto old{static_cast<PtrType>(*this)};
        write(read() + SIZE);
        return old;
    }

    MUTABLE_ONLY_
    const PtrType operator--() const
    {
        write(read() - SIZE);
        return static_cast<PtrType>(*this);
    }

    MUTABLE_ONLY_
    const PtrType operator--(int) const
    {
        auto old{static_cast<PtrType>(*this)};
        write(read() - SIZE);
        return old;
    }

    #undef IF_CONVERTIBLE_
    #undef MUTABLE_ONLY_

private:
    AddrType read() const
    {
        return mem_hdl_.template read<AddrType>(addr_);
    }

    void write(AddrType val) const
    {
        mem_hdl_.template write<AddrType>(addr_, val);
    }

    mutable HandleType mem_hdl_;
    AddrType addr_;
};

} // Mem64

