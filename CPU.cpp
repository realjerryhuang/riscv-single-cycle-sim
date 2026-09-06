#include "CPU.h"

CPU::CPU()
{
	pc = 0;
	lastFetchedInst = 0;
}

uint32_t CPU::readPC() const
{
	return pc;
}

uint32_t CPU::readReg(unsigned int regNum) const
{
	return cpuRegFile.readReg(regNum);
}

void CPU::loadProgram(const std::vector<uint8_t>& program)
{
	for (size_t i = 0; i+3 < program.size(); i+=4) {
		uint32_t word = (static_cast<uint32_t>(program[i])) |
			(static_cast<uint32_t>(program[i+1]) << 8) |
			(static_cast<uint32_t>(program[i+2]) << 16) |
			(static_cast<uint32_t>(program[i+3]) << 24);

		cpuInstMem.writeInst(static_cast<uint32_t>(i), word);
	}
}

void CPU::cycle()
{
	CycleState myCycleState{};	// Zero-initialized--erm...maybe not hardware accurate...

	fetch(myCycleState);
	lastFetchedInst = myCycleState.inst;
	decode(myCycleState);
	execute(myCycleState);
	memAccess(myCycleState);
	writeBack(myCycleState);
}

uint32_t CPU::readLastFetchedInst() const
{
	return lastFetchedInst;
}

void CPU::fetch(CycleState& myCycleState)
{
	// Fetch instruction from instruction memory
	myCycleState.inst = cpuInstMem.readInst(pc);
	
	// Increment PC by one instruction (four bytes)
	myCycleState.pcPlus4 = pc + 4;

	return;
}

void CPU::decode(CycleState& myCycleState)
{
    uint32_t inst = myCycleState.inst;

	// Split instruction into fields
	/*	0x  0     0     0     0     0     0   0   0
		  31-28 27-24 23-20 19-16 15-12 11-8 7-4 3-0	*/
	uint32_t opcode = inst & 0x0000007F;					// 6:0
	uint32_t readReg1 = (inst & 0x000F8000) >> 15;			// 19:15 (AKA rs1)
	uint32_t readReg2 = (inst & 0x01F00000) >> 20;			// 24:20 (AKA rs2)
	myCycleState.writeReg = (inst & 0x00000F80) >> 7;		// 11:7 (AKA rd)
	uint32_t inst30 = (inst & 0x40000000) >> 30;			// 30 (AKA funct7 bit)
	myCycleState.inst14to12 = (inst & 0x00007000) >> 12;	// 14:12 (AKA funct3)

	// Instruction to control
	control(opcode, 
			myCycleState.branch, 
			myCycleState.jump, 
			myCycleState.memRead, 
			myCycleState.resultSrc, 
			myCycleState.aluOp, 
			myCycleState.memWrite, 
			myCycleState.aluSrc, 
			myCycleState.regWrite
	);

	// Control to ALU control
	aluControl(myCycleState.aluOp, inst30, myCycleState.inst14to12, myCycleState.aluCtrlOut);

	// Instruction to register file
	myCycleState.rs1Data = cpuRegFile.readReg(readReg1);
	myCycleState.rs2Data = cpuRegFile.readReg(readReg2);

	// Instruction to immediate generation
	myCycleState.imm = immGen(inst);

	return;
}

void CPU::execute(CycleState& myCycleState)
{
	// Determine ALU op2
	uint32_t op2 = mux2to1<uint32_t>(myCycleState.rs2Data, myCycleState.imm, myCycleState.aluSrc);

	// ALU computation
	myCycleState.cpuALUOutput = aluComputation(myCycleState.rs1Data, op2, static_cast<ALUOperation>(myCycleState.aluCtrlOut));

	// Immediate left-shift
	int32_t shiftedImm = shiftLeft1(myCycleState.imm);
	
	// Branching
	uint32_t pcPlusImm = pc + shiftedImm;
	bool branchTaken = myCycleState.branch & !myCycleState.cpuALUOutput.zero;
	uint32_t branchMUX1Result = mux2to1<uint32_t>(myCycleState.pcPlus4, pcPlusImm, branchTaken);
	uint32_t jalrTarget = myCycleState.cpuALUOutput.result & ~1u;		// Force LSB to be 0
	myCycleState.branchMUX2Result = mux2to1(branchMUX1Result, jalrTarget, myCycleState.jump);

	return;
}

void CPU::memAccess(CycleState& myCycleState)
{
	if (myCycleState.memRead) {
		if (myCycleState.inst14to12 == 0b100)		// LBU
			myCycleState.dataMemOut = cpuDataMem.readByte(myCycleState.cpuALUOutput.result);
		else if (myCycleState.inst14to12 == 0b010)	// LW
			myCycleState.dataMemOut = cpuDataMem.readWord(myCycleState.cpuALUOutput.result);
	}
	else if (myCycleState.memWrite) {
		if (myCycleState.inst14to12 == 0b001)		// SH
			cpuDataMem.writeHalf(myCycleState.cpuALUOutput.result, myCycleState.rs2Data);
		else if (myCycleState.inst14to12 == 0b010)	// SW
			cpuDataMem.writeWord(myCycleState.cpuALUOutput.result, myCycleState.rs2Data);
	}

	return;
}

void CPU::writeBack(CycleState& myCycleState)
{
	// Update PC
	pc = myCycleState.branchMUX2Result;

	// Write to register file
	uint32_t rfMUXResult = mux4to1(myCycleState.cpuALUOutput.result, myCycleState.dataMemOut, myCycleState.pcPlus4, 0u, myCycleState.resultSrc);
	if (myCycleState.regWrite)
		cpuRegFile.writeReg(myCycleState.writeReg, rfMUXResult);
	return;
}