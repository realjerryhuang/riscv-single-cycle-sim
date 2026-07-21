#include "RegFile.h"
#include <cassert>

RegFile::RegFile() : x{}        // Initialize all register values to 0
{
}

void RegFile::writeReg(unsigned int regNum, uint32_t data)
{
    assert(regNum < 32);        // Check that regNum is within range [0, 31]
    if (regNum == 0)            // x0 is hardcoded to 0
        return;
    else
        x[regNum] = data;
}

uint32_t RegFile::readReg(unsigned int regNum) const
{
    assert(regNum < 32);        // Check that regNum is within range [0, 31]
    return x[regNum];
}