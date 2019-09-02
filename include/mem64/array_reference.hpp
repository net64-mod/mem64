#pragma once

#include "reference_common.hpp"


namespace Mem64
{

/// Reference to array
template<typename TType, typename THandle>
struct Ref<TType, THandle,
    std::enable_if_t<std::is_array_v<TType>>>
    : RefBase<Ref<TType, THandle>>
{
    using Traits = RefTraits<Ref<TType, THandle>>;
    using HandleType = typename Traits::HandleType;
    using AddrType = typename Traits::AddrType;
    using USizeType = typename Traits::USizeType;
    using QualifiedType = typename Traits::QualifiedType;


    explicit Ref(const HandleType& hdl, AddrType addr = HandleType::INVALID_OFFSET):
    RefBase<Ref<TType, THandle>>(hdl, addr)
    {}

    Ref<std::remove_extent_t<QualifiedType>, HandleType> operator[](USizeType i) const
    {
        return Ref<std::remove_extent_t<QualifiedType>, HandleType>{
            this->mem_hdl_, this->addr_ + hdl_sizeof_v<std::remove_extent_t<QualifiedType>, HandleType> * i
        };
    }
};

} // Mem64

