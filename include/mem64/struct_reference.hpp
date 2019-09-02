#pragma once

#include "reference_common.hpp"


namespace Mem64
{

/// Reference to struct
template<typename TType, typename THandle>
struct Ref<TType, THandle,
    std::enable_if_t<std::is_standard_layout_v<TType> && !std::is_fundamental_v<TType> && !std::is_enum_v<TType> &&
                     !is_nested_ptr_v<TType> && !std::is_array_v<TType>>>
    : RefBase<Ref<TType, THandle>>
{
    using Traits = RefTraits<Ref<TType, THandle>>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using USizeType = typename Traits::USizeType;
    using RawType = typename Traits::RawType;
    using QualifiedType = typename Traits::QualifiedType;

    template<typename T>
    using Qualified = std::conditional_t<RefTraits<Ref<TType, THandle>>::IS_CONST,
                          std::add_const_t<T>,
                      T>;

    explicit Ref(const HandleType& hdl, AddrType addr = HandleType::INVALID_OFFSET):
    RefBase<Ref<TType, THandle>>(hdl, addr)
    {}

    template<typename TMember>
    const auto field(TMember (RawType::*const member)) const
    {
        return Ref<Qualified<TMember>, HandleType>(this->mem_hdl_, this->addr_ + offset_of<USizeType>(member));
    }
};

} // Mem64
