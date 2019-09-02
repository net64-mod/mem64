#pragma once


namespace Mem64
{

template<typename T>
struct OperatorProxy
{
    OperatorProxy(T val)
    :val_{std::move(val)}
    {}

    const T* operator->()
    {
        return &val_;
    }

private:
    T val_;
};

template<typename T, typename U, typename V>
T offset_of(V U::*const ptr)
{
    const U val{};
    return static_cast<T>(
        reinterpret_cast<std::uintptr_t>(&(val.*ptr)) - reinterpret_cast<std::uintptr_t>(&val)
    );
}

template<template<typename...>typename TTemplate, typename T>
struct IsInstantiationOf : std::false_type{};

template<template<typename...>typename TTemplate, typename... TArgs>
struct IsInstantiationOf<TTemplate, TTemplate<TArgs...>> : std::true_type{};


/// Check if TType is an instantiation of TTemplate
template<template<typename...>typename TTemplate, typename TType>
constexpr bool is_instantiation_of_v{IsInstantiationOf<TTemplate, TType>::value};

} // Mem64
