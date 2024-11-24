#pragma once
#include "Constants.h"
#include "Enums.h"
#include "Utils.h"
#include "span"
#include "vector"

namespace NES
{
    class CPU
    {
    public:
        CPU() { mMemory.resize(Constant::kMemoryEnd); }

        /**
         * Reads the data stored in the CPU memory, by default reads at the program counter
         * @param address The address to read at
         */
        [[nodiscard]] uint8_t ReadMemory(const uint16_t address) const { return mMemory[address]; }

        /**
         * Writes data to a given location in CPU memory
         * @param address The address to write at
         * @param data The data to write
         */
        void WriteMemory(const uint16_t address, const uint8_t data) { mMemory[address] = data; }

        /**
         * Reads memory address as a uint16 as little endian
         * i.e. with the least significant bytes placed at the lowest address
         * @param address The address to read from
         */
        [[nodiscard]] uint16_t ReadMemoryAsU16(const uint16_t address) const
        {
            const auto lowBits = static_cast<uint16_t>(ReadMemory(address));
            const auto highBits = static_cast<uint16_t>(ReadMemory(address + 1));
            return highBits << 8 | lowBits;
        }

        /**
         * Writes to a memory address in little endian format
         * i.e. with the least significant bytes placed at the lowest address
         * @param address The address at which to write
         * @param data The data to write
         */
        void WriteMemoryAsU16(const uint16_t address, const uint16_t data)
        {
            const auto lowBits = static_cast<uint8_t>(data & 0xFF);
            const auto highBits = static_cast<uint8_t>(data >> 8);
            WriteMemory(address, lowBits);
            WriteMemory(address + 1, highBits);
        }

        /**
         * Loads an arbitrary rom into cpu memory at the required location
         * @param rom A container of bytes containing the ram data
         */
        void LoadROM(std::span<uint8_t> rom)
        {
            std::ranges::copy(rom, mMemory.begin() + Constant::kRomStartAddress);
            WriteMemoryAsU16(0xFFFC, 0x8000);
        }

        /**
         * Restores the state of all registers
         */
        void Reset()
        {
            mRegisterA = 0;
            mRegisterX = 0;
            mStatus = 0;
            mProgramCounter = ReadMemoryAsU16(0xFFFC);
        }

        /**
         *
         */
        [[nodiscard]] uint16_t GetAddress(const AddressingMode mode) const
        {
            uint8_t pos = 0;
            uint8_t lowBits = 0;
            uint8_t highBits = 0;
            uint16_t base = 0;
            uint16_t address = 0;

            switch (mode)
            {
                case AddressingMode::eImmediate:
                    return mProgramCounter;
                case AddressingMode::eZeroPage:
                    return ReadMemory(mProgramCounter);
                case AddressingMode::eZeroPageX:
                    pos = ReadMemory(mProgramCounter);
                    address = Utils::WrapAdd(pos, mRegisterX);
                    return address;
                case AddressingMode::eZeroPageY:
                    pos = ReadMemory(mProgramCounter);
                    address = Utils::WrapAdd(pos, mRegisterY);
                    return address;
                case AddressingMode::eAbsolute:
                    return ReadMemoryAsU16(mProgramCounter);
                case AddressingMode::eAbsoluteX:
                    base = ReadMemoryAsU16(mProgramCounter);
                    address = Utils::WrapAdd(base, static_cast<uint16_t>(mRegisterX));
                    return address;
                case AddressingMode::eAbsoluteY:
                    base = ReadMemoryAsU16(mProgramCounter);
                    address = Utils::WrapAdd(base, static_cast<uint16_t>(mRegisterY));
                    return address;
                case AddressingMode::eIndirectX:
                    pos = Utils::WrapAdd(ReadMemory(mProgramCounter), mRegisterX);
                    lowBits = ReadMemory(pos);
                    highBits = ReadMemory(Utils::WrapAdd(pos, static_cast<uint8_t>(1)));
                    return static_cast<uint16_t>(highBits) << 8 | static_cast<uint16_t>(lowBits);
                case AddressingMode::eIndirectY:
                    pos = Utils::WrapAdd(ReadMemory(mProgramCounter), mRegisterY);
                    lowBits = ReadMemory(pos);
                    highBits = ReadMemory(Utils::WrapAdd(pos, static_cast<uint8_t>(1)));
                    base = static_cast<uint16_t>(highBits) << 8 | static_cast<uint16_t>(lowBits);
                    return Utils::WrapAdd(base, static_cast<uint16_t>(mRegisterY));
            }
            // Should never reach this
            return -1;
        }

        /**
         * Loads an instruction into register A
         * Sets the zero flag is A = 0
         * Sets the negative flag if bit 7 is set
         */
        void LDA(const AddressingMode addressingMode)
        {
            const auto address = GetAddress(addressingMode);
            mRegisterA = ReadMemory(address);

            mStatus = Utils::SetFlagStatus(mStatus, Constant::kZeroFlag, mRegisterA == 0);
            mStatus = Utils::SetFlagStatus(mStatus, Constant::kNegativeFlag, mRegisterA & Constant::kNegativeFlag);
        }

        /**
         * Loads an instruction into register X
         * Sets the zero flag is X = 0
         * Sets the negative flag if bit 7 is set
         */
        void LDX(const AddressingMode mode)
        {
            const auto address = GetAddress(mode);
            mRegisterX = ReadMemory(address);

            mStatus = Utils::SetFlagStatus(mStatus, Constant::kZeroFlag, mRegisterX == 0);
            mStatus = Utils::SetFlagStatus(mStatus, Constant::kNegativeFlag, mRegisterX & Constant::kNegativeFlag);
        }

        /**
         * Copies the accumulator contents into register X
         * Sets the zero flag is X = 0
         * Sets the negative flag if bit 7 is set
         */
        void TAX()
        {
            mRegisterX = mRegisterA;
            mStatus = Utils::SetFlagStatus(mStatus, Constant::kZeroFlag, mRegisterX == 0);
            mStatus = Utils::SetFlagStatus(mStatus, Constant::kNegativeFlag, mRegisterX & Constant::kNegativeFlag);
        }

        /**
        * Copes the value from register A to memory
        */
        void STA(const AddressingMode mode)
        {
            const auto address = GetAddress(mode);
            WriteMemory(address, mRegisterA);
        }


        uint8_t mRegisterA = 0;
        uint8_t mRegisterX = 0;
        uint8_t mRegisterY = 0;
        uint8_t mStatus = 0;
        uint16_t mProgramCounter = 0;
        std::vector<uint8_t> mMemory;
    };
} // namespace NES
