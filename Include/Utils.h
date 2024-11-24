#pragma once
#include "limits"

namespace NES::Utils
{
    inline uint8_t SetFlagStatus(const uint8_t status, const uint8_t mask, const bool value)
    {
        return value ? status | mask : status & ~mask;
    }

    /**
    * Adds two values of Type, which wraps around the numeric limits of Type
    */
    template<typename Type>
    Type WrapAdd(Type left, Type right)
    {
        return (left + right) % (std::numeric_limits<Type>::max() + 1);
    }
}