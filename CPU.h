#ifndef CPU_INCLUDED
#define CPU_INCLUDED

#include <iostream>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cassert>

#include "memory.h"
#include "RegFile.h"
#include "ALU.h"
#include "mux.h"
#include "immGen.h"
#include "shiftLeft1.h"
#include "control.h"

class CPU 
{
    public:
        CPU();
        uint32_t readPC() const;
        uint32_t readReg(unsigned int regNum) const; 
        void loadProgram(const std::vector<uint8_t>& program);
        void cycle();

    private:
        uint32_t pc;
        RegFile cpuRegFile;
        InstructionMemory cpuInstMem;
        DataMemory cpuDataMem;

        struct CycleState
        {
            uint32_t inst;
            uint32_t pcPlus4;
            
            // Decode outputs
            uint32_t writeReg;
            uint32_t rs1Data;
            uint32_t rs2Data;
            int32_t imm;
            uint32_t inst14to12;    // AKA funct3

            // Control signals
            uint32_t branch;
            uint32_t jump;
            uint32_t memRead;
            uint32_t resultSrc;
            uint32_t aluOp;
            uint32_t memWrite;
            uint32_t aluSrc;
            uint32_t regWrite;
            uint32_t aluCtrlOut;

            // Execute outputs
            ALUOutput cpuALUOutput;
            uint32_t branchMUX2Result;

            // Memory access outputs
            uint32_t dataMemOut;

        };

        /* Instruction cycle states make up FSM... */
        void fetch(CycleState& myCycleState);       // S0
        void decode(CycleState& myCycleState);      // S1
        void execute(CycleState& myCycleState);     // S2
        void memAccess(CycleState& myCycleState);   // S3
        void writeBack(CycleState& myCycleState);   // S4

};

#endif  // CPU_INCLUDED