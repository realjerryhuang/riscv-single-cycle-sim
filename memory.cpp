#include "memory.h"

/* INSTRUCTION MEMORY */
/* Initialize instruction memory by filling cells with all zeros. */
InstructionMemory::InstructionMemory() : instMem{}
{
}

/* Given an address and instruction, store the instruction at that address in little-endian format. */
void InstructionMemory::writeInst(uint32_t addr, uint32_t inst)
{
    assert(addr % 4 == 0 && "Instruction address must be word-aligned");
    assert(addr + 3 < INST_MEM_SIZE && "Instruction address out of bounds");

    uint8_t byte3 = (inst >> 24) & 0xFF;    // Right shift 24 bits to align top 8 bits to the right, then mask to isolate those 8 bits
    uint8_t byte2 = (inst >> 16) & 0xFF;
    uint8_t byte1 = (inst >> 8) & 0xFF;
    uint8_t byte0 = inst & 0xFF;

    instMem[addr] = byte0;
    instMem[addr+1] = byte1;
    instMem[addr+2] = byte2;
    instMem[addr+3] = byte3;
}

/* Given an address, reconstruct and return the instruction stored in the instruction memory at that address. */
uint32_t InstructionMemory::readInst(uint32_t addr) const
{
    assert(addr % 4 == 0 && "Instruction address must be word-aligned");
    assert(addr + 3 < INST_MEM_SIZE && "Instruction address out of bounds");

    // Return 4 bytes of instMem array starting at the index given by addr
    uint8_t byte0 = instMem[addr];
    uint8_t byte1 = instMem[addr+1];
    uint8_t byte2 = instMem[addr+2];
    uint8_t byte3 = instMem[addr+3];

    return (static_cast<uint32_t>(byte3) << 24) | 
        (static_cast<uint32_t>(byte2) << 16) | 
        (static_cast<uint32_t>(byte1) << 8) | 
        (static_cast<uint32_t>(byte0));
}


/* DATA MEMORY */
/* Instantiate data memory by filling cells with all zeros. */
DataMemory::DataMemory() : dataMem{}
{
}

/* Given an address and 16-bit data, store the data at that address. */
void DataMemory::writeHalf(uint32_t addr, uint16_t half)
{
    assert(addr % 2 == 0 && "Half-word address must be 2-byte aligned");
    assert(addr + 1 < DATA_MEM_SIZE && "Half-word address out of bounds");

    uint8_t byte1 = (half >> 8) & 0xFF;
    uint8_t byte0 = half & 0xFF;

    dataMem[addr] = byte0;
    dataMem[addr+1] = byte1;
}

/* Given an address and 32-bit data, store the data at that address. */
void DataMemory::writeWord(uint32_t addr, uint32_t word)
{
    assert(addr % 4 == 0 && "Word address must be 4-byte aligned");
    assert(addr + 3 < DATA_MEM_SIZE && "Word address out of bounds");

    uint8_t byte3 = (word >> 24) & 0xFF;
    uint8_t byte2 = (word >> 16) & 0xFF;
    uint8_t byte1 = (word >> 8) & 0xFF;
    uint8_t byte0 = word & 0xFF;

    dataMem[addr] = byte0;
    dataMem[addr+1] = byte1;
    dataMem[addr+2] = byte2;
    dataMem[addr+3] = byte3;
}

/* Given an address, return the 8-bit data stored in the data memory at that address. */
uint8_t DataMemory::readByte(uint32_t addr) const
{
    // Byte address must be 1-byte aligned, so we're good here
    assert(addr < DATA_MEM_SIZE && "Byte address out of bounds");

    return dataMem[addr];
}

/* Given an address, return the 32-bit data stored in the data memory at that address. */
uint32_t DataMemory::readWord(uint32_t addr) const
{
    assert(addr % 4 == 0 && "Word address must be 4-byte aligned");
    assert(addr + 3 < DATA_MEM_SIZE && "Word address out of bounds");

    uint8_t byte0 = dataMem[addr];
    uint8_t byte1 = dataMem[addr+1];
    uint8_t byte2 = dataMem[addr+2];
    uint8_t byte3 = dataMem[addr+3];

    return (static_cast<uint32_t>(byte3) << 24) | 
        (static_cast<uint32_t>(byte2) << 16) | 
        (static_cast<uint32_t>(byte1) << 8) | 
        (static_cast<uint32_t>(byte0));
}