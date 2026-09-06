#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "CPU.h"

const int MAX_CYCLES = 1000000;	// Generous upper bound for expected test programs

std::string disassemble(uint32_t inst);


int main(int argc, char* argv[]) 
{
    /* Check if the instruction file is any good. */
	if (argc < 2) {
        std::cout << "No file name entered. Exiting...";
        return -1;
    }

    std::ifstream infile(argv[1]);      // Open the file
    if (!(infile.is_open() && infile.good())) {
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

	/* Load in the instruction file. */
	std::vector<uint8_t> program;
    std::string line;

	while (infile >> line) {
		// Convert each line from a char interpretation to an int interpretation to an 8-bit interpretation
		uint8_t byte = static_cast<uint8_t>(std::stoi(line, nullptr, 16));
		program.push_back(byte);
	}
    
	uint32_t programSize = static_cast<uint32_t>(program.size());

    /* Instantiate CPU object. */
    CPU myCPU;
	myCPU.loadProgram(program);

	/* Run simulator. */
	int cycleCount = 0;
	std::cout << "Program START" << std::endl;
    while (myCPU.readPC() < programSize) {      // Processor's main loop; each iteration is equal to one CLK cycle.
        uint32_t currentPC = myCPU.readPC();
		myCPU.cycle();
		uint32_t currentInst = myCPU.readLastFetchedInst();

		std::cout << "Cycle " << std::setw(5) << std::right << cycleCount << ":"
			<< "\tPC = 0x" << std::hex << std::setfill('0') << std::setw(4) << std::right << currentPC
			<< std::dec << std::setfill(' ')
			<< "\tInst = 0x" << std::hex << std::setfill('0') << std::setw(8) << std::right << currentInst
			<< std::dec << std::setfill(' ')
			<< "\tAsm: " << disassemble(currentInst)
			<< std::endl;

		cycleCount++;
		if (cycleCount > MAX_CYCLES) {
			std::cerr << "Exceeded max cycle count." << std::endl;
			break;
		}
    }
	std::cout << "Program END" << std::endl;

    /* Print function arguments/ return values. */
	int a0 = static_cast<int>(myCPU.readReg(10));
    int a1 = static_cast<int>(myCPU.readReg(11));
	std::cout << "a0 = " << a0 << ", a1 = " << a1 << std::endl;

    return 0;
}


std::string disassemble(uint32_t inst)
{
	uint32_t opcode = inst & 0x7F;
	uint32_t rd = (inst >> 7) & 0x1F;
	uint32_t funct3 = (inst >> 12) & 0x7;
	uint32_t rs1 = (inst >> 15) & 0x1F;
	uint32_t rs2 = (inst >> 20) & 0x1F;
	uint32_t funct7Bit = (inst >> 30) & 0x1;

	std::ostringstream oss;

	switch (opcode) {
		case 0b0110011:	{	// R-type: AND, SUB, SRA
			const char* mnemonic = "unknown";
			if (funct3 == 0b000) 
				mnemonic = "sub";
			else if (funct3 == 0b101)
				mnemonic = funct7Bit ? "sra" : "srl";
			else if (funct3 == 0b111)
				mnemonic = "and";
			oss << mnemonic << " x" << rd << " x" << rs1 << " x" << rs2;
			break;
		}
		case 0b0010011: {	// I-type ALU: ADDI, ORI, SLTIU
			int32_t imm = immGen(inst);
			const char* mnemonic = "unknown";
			if (funct3 == 0b000)
				mnemonic = "addi";
			else if (funct3 == 0b011)
				mnemonic = "sltiu";
			else if (funct3 == 0b110)
				mnemonic = "ori";
			oss << mnemonic << " x" << rd << " x" << rs1 << " " << imm;
			break;
		}
		case 0b0000011: {	// Loads: LBU, LW
			int32_t imm = immGen(inst);
			const char* mnemonic = (funct3 == 0b100) ? "lbu" : (funct3 == 0b010) ? "lw" : "unknown";
			oss << mnemonic << " x" << rd << " " << imm << " x" << rs1;
			break;
		}
		case 0b1100111: {	// JALR
			int32_t imm = immGen(inst);
			oss << "jalr x" << rd << " x" << rs1 << " " << imm;
			break;
		}
		case 0b0100011: {	// Stores: SH, SW
			int32_t imm = immGen(inst);
			const char* mnemonic = (funct3 == 0b001) ? "sh" : (funct3 == 0b010) ? "sw" : "unknown";
			oss << mnemonic << " x" << rs2 << " " << imm << " x" << rs1;
			break;
		}
		case 0b1100011: {	// BNE (properly shifted)
			int32_t imm = shiftLeft1(immGen(inst));
			oss << "bne x" << rs1 << " x" << rs2 << " " << imm;
			break;
		}
		case 0b0110111: {	// LUI
			int32_t imm = immGen(inst);
			oss << "lui x" << rd << " 0x" << std::hex << (static_cast<uint32_t>(imm) >> 12) << std::dec;
			break;
		}
		default:
			oss << "unknown";
	}

	return oss.str();
}