#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

#include <cstdint>

constexpr unsigned int INST_MEM_SIZE = 4096;
constexpr unsigned int DATA_MEM_SIZE = 4096;

class InstructionMemory
{
    public:
        InstructionMemory();
        void writeInst(uint32_t addr, uint32_t inst);
        uint32_t readInst(uint32_t addr) const;
    private:
        std::uint8_t instMem[INST_MEM_SIZE];    // Memory holds 4096 cells, each storing 1 byte (8 bits)
};

class DataMemory
{
    public:
        DataMemory();
    private:
        int dataMem[DATA_MEM_SIZE];

};

#endif  // MEMORY_INCLUDED