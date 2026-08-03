#ifndef CONTROL_INCLUDED
#define CONTROL_INCLUDED

#include <cstdint>

enum ALUOperation : uint32_t {
    ALU_AND = 0,
    ALU_OR,
    ALU_ADD,
    ALU_SUB,
    ALU_SLL,
    ALU_SLT,
    ALU_SLTU,
    ALU_XOR,
    ALU_SRL,
    ALU_SRA,
};

void control
(
	uint32_t opcode, 
	uint32_t& branch, 
	uint32_t& memRead, 
	uint32_t& memToReg, 
	uint32_t& aluOp, 
	uint32_t& memWrite, 
	uint32_t& aluSrc, 
	uint32_t& regWrite
);

void aluControl
(
	uint32_t aluOp,
	uint32_t inst30,
	uint32_t inst14to12,
	uint32_t& aluCtrlOut
);

#endif