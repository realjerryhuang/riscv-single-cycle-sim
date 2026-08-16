#ifndef IMMGEN_INCLUDED
#define IMMGEN_INCLUDED

#include <cstdint>

inline int32_t immGen(uint32_t inst)
{
	uint32_t opcode = inst & 0x0000007F;
	int32_t immediate = 0;
	
	/* R-type: SRA, SUB, AND */
	if (opcode == 0b0110011) {
		immediate = 0;		// Irrelevant for R-type
	}
	/* I-type: ADDI, ORI, SLTIU; LBU, LW; JALR */
	else if (opcode == 0b0010011 || opcode == 0b0000011 || opcode == 0b1100111) {
		int32_t imm11to0 = inst & 0xFFF00000;
		immediate = imm11to0 >> 20;
	}
	/* S-type: SH, SW */
	else if (opcode == 0b0100011) {
		int32_t imm11to5 = inst & 0xFE000000;
		int32_t imm4to0 = inst & 0x00000F80;
		immediate = (imm4to0 >> 7) | (imm11to5 >> 20);
	}
	/* B-type: BNE */
	else if (opcode == 0b1100011) {
		int32_t imm12 = inst & 0x80000000;
		int32_t imm10to5 = inst & 0x7E000000;
		int32_t imm4to1 = inst & 0x00000F00;
		int32_t imm11 = inst & 0x00000080;
		immediate = (imm12 >> 19) | (imm10to5 >> 20) | (imm4to1 >> 7) | (imm11 << 4);
	}
	/* U-type: LUI */
	else if (opcode == 0b0110111) {
		immediate = inst & 0xFFFFF000;
	}
	
	return immediate;
}

#endif	// IMMGEN_INCLUDED