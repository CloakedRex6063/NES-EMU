#pragma once
#include "cstdint"

namespace NES::Constant
{
    constexpr uint8_t kZeroFlag = 0b0000'0010;
    constexpr uint8_t kNegativeFlag = 0b1000'0000;
    constexpr uint16_t kResetAddress = 0xFFFC;
    constexpr uint16_t kRomStartAddress = 0x8000;
    constexpr uint16_t kMemoryEnd = 0xFFFF;
}