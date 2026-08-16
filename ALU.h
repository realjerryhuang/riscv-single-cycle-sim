#ifndef ALU_INCLUDED
#define ALU_INCLUDED

#include <cstdint>
#include <iostream>
#include "control.h"        // For the ALU operation enumeration

struct ALUOutput
{
    uint32_t result;
    bool zero;
};

ALUOutput aluComputation(uint32_t operand1, uint32_t operand2, ALUOperation aluCtrlOut);

#endif  // ALU_INCLUDED