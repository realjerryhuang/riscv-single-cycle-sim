#include "ALU.h"

ALUOutput aluComputation(uint32_t operand1, uint32_t operand2, ALUOperation aluCtrlOut)
{
    ALUOutput newOutput;

    switch(aluCtrlOut) {
        case ALU_AND:
            newOutput.result = operand1 & operand2;
            break;
        case ALU_OR:
            newOutput.result = operand1 | operand2;
            break;
        case ALU_ADD:
            newOutput.result = operand1 + operand2;
            break;
        case ALU_SUB:
            newOutput.result = operand1 - operand2;
            break;
        case ALU_SLTU:      // "Set Less Than Unsigned"
            newOutput.result = (operand1 < operand2) ? 1 : 0;
            break;
        case ALU_SRL: {
            uint32_t shiftAmt = operand2 & 0x0000001F;      // Take only the bottom five bits of operand2 for a max shift of 31
            newOutput.result = operand1 >> shiftAmt;
            break;
        }
        case ALU_SRA: {
            uint32_t shiftAmt = operand2 & 0x0000001F;
            newOutput.result = static_cast<uint32_t>(static_cast<int32_t>(operand1) >> shiftAmt);
            break;
        }
        default:            // Unsupported operation defaults to add
            std::cerr << "ERROR: Invalid ALU control signal." << std::endl;
            newOutput.result = operand1 + operand2;
            break;
    }
    
    newOutput.zero = (newOutput.result == 0);

    return newOutput;
}