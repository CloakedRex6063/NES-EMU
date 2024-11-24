#include "CPU.h"
#include "print"

int main()
{
    std::vector<uint8_t> mRom{0xA9, 0x10, 0x00};
    NES::CPU cpu;
    cpu.LoadROM(mRom);
    cpu.Reset();

    while (true)
    {
        const auto opCode = cpu.ReadMemory(cpu.mProgramCounter);
        cpu.mProgramCounter++;

        std::print("Opcode: {} \n", opCode);
        switch (opCode)
        {
            // force interrupts the program, breaking out of the infinite loop
            case 0x00: // BRK
                return 0;
            case 0xA9:
                cpu.LDA(NES::AddressingMode::eImmediate);
                cpu.mProgramCounter++;
                break;
            case 0xA5:
                cpu.LDA(NES::AddressingMode::eZeroPage);
                cpu.mProgramCounter++;
                break;
            case 0xAD:
                cpu.LDA(NES::AddressingMode::eAbsolute);
                cpu.mProgramCounter+=2;
                break;
            case 0xAA:
                cpu.TAX();
                break;
            case 0x85:
                cpu.STA(NES::AddressingMode::eZeroPage);
                cpu.mProgramCounter++;
            case 0x95:
                cpu.STA(NES::AddressingMode::eZeroPageX);
                cpu.mProgramCounter++;
            default:
                break;
        }
    }
}
