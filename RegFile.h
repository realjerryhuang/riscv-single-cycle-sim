#ifndef REGFILE_INCLUDED
#define REGFILE_INCLUDED

#include <cstdint>

class RegFile
{
    public:
        RegFile();
        void writeReg(unsigned int regNum, uint32_t data);
        uint32_t readReg(unsigned int regNum) const;
    private:
        uint32_t x[32];     // Registers x0 - x31
};

#endif  // REGFILE_INCLUDED