#include "control.h"

void control
(
	// Input
	uint32_t opcode,
	// Output control signals
	uint32_t& branch,
	uint32_t& memRead,
	uint32_t& memToReg,
	uint32_t& aluOp,
	uint32_t& memWrite,
	uint32_t& aluSrc,
	uint32_t& regWrite
)
{
	// Assume that opcode is 7 bits long

	/* Should support:
	ADDI, LUI, ORI, SLTIU, SRA, SUB, AND 
	BNE, JALR */

	/* R-type: SRA, SUB, AND */
	if (opcode == 0b0110011) {
		branch = 0b0;
		memRead = 0b0;
		memToReg = 0b0;
		aluOp = 0b10;
		memWrite = 0b0;
		aluSrc = 0b0;
		regWrite = 0b1;
	}
	/* I-type: ADDI, ORI, SLTIU */
	else if (opcode == 0b0010011) {
		branch = 0b0;
		memRead = 0b0;
		memToReg = 0b0;
		aluOp = 0b10;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* I-type: LBU, LW */
	else if (opcode == 0b0000011) {
		branch = 0b0;
		memRead = 0b1;
		memToReg = 0b1;
		aluOp = 0b00;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* I-type: JALR */
	else if (opcode == 0b1100111) {		// Iffy...
		branch = 0b1;
		memRead = 0b0;
		memToReg = 0b0;
		aluOp = 0b00;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* S-type: SH, SW */
	else if (opcode == 0b0100011) {
		branch = 0b0;
		memRead = 0b0;
		memToReg = 0b0;			// Don't care
		aluOp = 0b00;
		memWrite = 1;
		aluSrc = 1;
		regWrite = 0;
	}
	/* B-type: BNE */
	else if (opcode == 0b1100011) {
		branch = 0b1;
		memRead = 0b0;
		memToReg = 0b0;			// Don't care
		aluOp = 0b01;
		memWrite = 0b0;
		aluSrc = 0b0;
		regWrite = 0b0;
	}
	/* U-type: LUI */
	else if (opcode == 0b0110111) {
		branch = 0b0;
		memRead = 0b0;
		memToReg = 0b0;
		aluOp = 0b00;			// Don't care
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;		
	}
	return;
}

void aluControl
(
	// Inputs
	uint32_t aluOp,
	uint32_t inst30,
	uint32_t inst14to12,
	// Output control signal
	uint32_t& aluCtrlOut
)
{
	/*
		aluOp == 00 -> sw/lw -> add
		aluOp == 01 -> beq -> sub
		aluOp == 10 -> r-type
		aluOp == 11 -> i-type
	*/
	if (aluOp == 00)

	// funct7
}