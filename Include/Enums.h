#pragma once

namespace NES
{
    enum class AddressingMode
    {
        eImmediate,
        eZeroPage,
        eZeroPageX,
        eZeroPageY,
        eAbsolute,
        eAbsoluteX,
        eAbsoluteY,
        eIndirectX,
        eIndirectY
    };
}