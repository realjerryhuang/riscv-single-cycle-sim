#include "control.h"

void control
(
	// Input
	uint32_t opcode,
	// Output control signals
	uint32_t& branch,
	uint32_t& jump,			// To support JALR
	uint32_t& memRead,
	uint32_t& resultSrc,	// New, two-bit wide version of memToReg
	uint32_t& aluOp,
	uint32_t& memWrite,
	uint32_t& aluSrc,
	uint32_t& regWrite
)
{
	// Assume that opcode is 7 bits long (To-Do: Check this.)
	/* R-type: SRA, SUB, AND */
	if (opcode == 0b0110011) {
		branch = 0b0;
		jump = 0b0;
		memRead = 0b0;
		resultSrc = 0b00;
		aluOp = 0b10;
		memWrite = 0b0;
		aluSrc = 0b0;
		regWrite = 0b1;
	}
	/* I-type: ADDI, ORI, SLTIU */
	else if (opcode == 0b0010011) {
		branch = 0b0;
		jump = 0b0;
		memRead = 0b0;
		resultSrc = 0b00;
		aluOp = 0b11;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* I-type: LBU, LW */
	else if (opcode == 0b0000011) {
		branch = 0b0;
		jump = 0b0;
		memRead = 0b1;
		resultSrc = 0b01;
		aluOp = 0b00;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* I-type: JALR */
	else if (opcode == 0b1100111) {
		branch = 0b0;
		jump = 0b1;
		memRead = 0b0;
		resultSrc = 0b10;
		aluOp = 0b00;
		memWrite = 0b0;
		aluSrc = 0b1;
		regWrite = 0b1;
	}
	/* S-type: SH, SW */
	else if (opcode == 0b0100011) {
		branch = 0b0;
		jump = 0b0;
		memRead = 0b0;
		resultSrc = 0b00;			// Don't care
		aluOp = 0b00;
		memWrite = 1;
		aluSrc = 1;
		regWrite = 0;
	}
	/* B-type: BNE */
	else if (opcode == 0b1100011) {
		branch = 0b1;
		jump = 0b0;
		memRead = 0b0;
		resultSrc = 0b00;			// Don't care
		aluOp = 0b01;
		memWrite = 0b0;
		aluSrc = 0b0;
		regWrite = 0b0;
	}
	/* U-type: LUI */
	else if (opcode == 0b0110111) {
		branch = 0b0;
		jump = 0b0;
		memRead = 0b0;
		resultSrc = 0b00;
		aluOp = 0b00;				// Don't care
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
	uint32_t funct3 = inst14to12;
	uint32_t funct7Bit = inst30;

	// To-Do: Check that funct3 is 3 bits and funct7Bit is 1 bit.

	switch (aluOp) {
		case 0b00:		// Add: LBU, LW, JALR, SH, SW, LUI
			aluCtrlOut = ALU_ADD;
			break;
		case 0b01:		// Subtract: BNE
			aluCtrlOut = ALU_SUB;
			break;
		case 0b10:		// R-type: SRA, SUB, AND
			switch (funct3) {
				case 0b000:
					aluCtrlOut = (funct7Bit == 0) ? ALU_ADD : ALU_SUB;
					break;
				case 0b101:
					aluCtrlOut = (funct7Bit == 0) ? ALU_SRL : ALU_SRA;
					break;
				case 0b110:
					aluCtrlOut = ALU_OR;
					break;
				case 0b111:
					aluCtrlOut = ALU_AND;
					break;
				default:
					std::cerr << "ERROR: Invalid R-type instruction." << std::endl;
					aluCtrlOut = ALU_ADD;
					break;
			}
			break;
		case 0b11:		// I-type: ADDI, ORI, SLTIU
			switch (funct3) {
				case 0b000:
					aluCtrlOut = ALU_ADD;
					break;
				case 0b011:
					aluCtrlOut = ALU_SLTU;
					break;
				case 0b110:
					aluCtrlOut = ALU_OR;
					break;
				case 0b111:
					aluCtrlOut = ALU_AND;
					break;
				default:
					std::cerr << "ERROR: Invalid I-type instruction." << std::endl;
					aluCtrlOut = ALU_ADD;
					break;
			}
			break;
		default:
			std::cerr << "ERROR: Invalid instruction." << std::endl;
			aluCtrlOut = ALU_ADD;
			break;
	}
	return;
}