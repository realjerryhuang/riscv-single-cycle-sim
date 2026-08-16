#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

#include <cstdint>

constexpr unsigned int INST_MEM_SIZE = 4096;    // 4 KB
constexpr unsigned int DATA_MEM_SIZE = 131072;  // 128 KB, originally 4 KB too but our traces' data addresses surpass that

class InstructionMemory
{
    public:
        InstructionMemory();
        void writeInst(uint32_t addr, uint32_t inst);   // Address is 32 bits because RISC-V PC (RV32I) is 32 bits
        uint32_t readInst(uint32_t addr) const;
    private:
        /* Byte-addressable instruction memory, addressable in little-endian fashion. */
        uint8_t instMem[INST_MEM_SIZE];    // Memory holds 4096 cells, each storing 1 byte (8 bits)
};

class DataMemory
{
    public:
        DataMemory();
        void writeHalf(uint32_t addr, uint16_t half);       // Supports SH instruction
        void writeWord(uint32_t addr, uint32_t word);       // Supports SW instruction
        uint8_t readByte(uint32_t addr) const;              // Supports LBU instruction
        uint32_t readWord(uint32_t addr) const;             // Supports LW instruction
    private:
        /* Byte-addressable data memory, addressable in little-endian fashion. */
        uint8_t dataMem[DATA_MEM_SIZE];
};

#endif  // MEMORY_INCLUDED