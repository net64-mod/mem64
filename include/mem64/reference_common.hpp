#pragma once

#include <type_traits>
#include "util.hpp"


namespace Mem64
{

template<typename T, typename THandle>
struct PtrTag
{
    using Type = T;
    typename THandle::addr_t padding;
};


template<typename, typename>
struct Ptr;


template<typename T, typename = void>
struct is_nested_ptr : std::false_type
{};

template<typename T>
struct is_nested_ptr<T, std::enable_if_t<std::is_pointer_v<T>>>
    : std::true_type
{};

template<typename T>
struct is_nested_ptr<T, std::enable_if_t<is_instantiation_of_v<PtrTag, T>>>
    : std::true_type
{};

template<typename T>
struct is_nested_ptr<T, std::enable_if_t<is_instantiation_of_v<Ptr, T>>>
    : std::true_type
{};

template<typename T>
constexpr bool is_nested_ptr_v = is_nested_ptr<T>::value;


template<typename>
struct remove_nested_ptr;

template<typename T>
struct remove_nested_ptr<T*>
{
    using Type = T;
};

template<typename T, typename U>
struct remove_nested_ptr<PtrTag<T, U>>
{
    using Type = T;
};

template<typename T, typename U>
struct remove_nested_ptr<Ptr<T, U>>
{
    using Type = T;
};

template<typename T>
using remove_nested_ptr_t = typename remove_nested_ptr<T>::Type;


template<typename T, typename U>
struct hdl_sizeof :
    std::integral_constant<typename U::usize_t, static_cast<typename U::usize_t>(sizeof(T))>
{};

template<typename T, typename U>
struct hdl_sizeof<T*, U> :
    std::integral_constant<typename U::usize_t,
                           static_cast<typename U::usize_t>(sizeof(typename U::addr_t))>
{};

template<typename T, typename U>
struct hdl_sizeof<Ptr<T, U>, U> :
    std::integral_constant<typename U::usize_t,
                           static_cast<typename U::usize_t>(sizeof(typename U::addr_t))>
{};

template<typename T, typename U>
constexpr auto hdl_sizeof_v{hdl_sizeof<T, U>::value};


template<typename>
struct RefTraits;

template<typename T, typename THandle, typename = void>
struct PtrTraits
{
    using HandleType = THandle;
    using AddrType = typename HandleType::addr_t;
    using SSizeType = typename HandleType::ssize_t;
    using USizeType = typename HandleType::usize_t;
    using RawType = T;
    using QualifiedType = std::remove_cv_t<T>;

    static constexpr USizeType SIZE{hdl_sizeof_v<QualifiedType, HandleType>};
};

template<typename TCrtp>
struct RefBase
{
    using Traits = RefTraits<TCrtp>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using USizeType = typename Traits::USizeType;
    using QualifiedType = typename Traits::QualifiedType;
    using RawType = typename Traits::RawType;

    friend TCrtp;

    operator typename Traits::template Rebind<const RawType>() const
    {
        return {mem_hdl_, addr_};
    }

    Ptr<QualifiedType, HandleType> ptr() const
    {
        return Ptr<QualifiedType, HandleType>{mem_hdl_, addr_};
    }

private:
    RefBase(HandleType hdl, AddrType addr):
    mem_hdl_{hdl}, addr_{addr}
    {}

    mutable HandleType mem_hdl_;
    AddrType addr_{};
};

template<typename, typename, typename = void>
struct Ref;

template<typename T, typename U>
struct RefTraits<Ref<T, U>>
{
    using HandleType = U;
    using AddrType = typename HandleType::addr_t;
    using USizeType = typename HandleType::usize_t;
    using SSizeType = typename HandleType::ssize_t;
    using QualifiedType = T;
    using RawType = std::remove_cv_t<T>;
    static constexpr bool IS_CONST{std::is_const_v<QualifiedType>};

    template<typename V>
    using Rebind = Ref<V, HandleType>;
};

} // Mem64

