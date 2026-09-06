#ifndef SHIFTLEFT1_INCLUDED
#define SHIFTLEFT1_INCLUDED

#include <cstdint>

inline int32_t shiftLeft1(int32_t imm) 
{
	return static_cast<int32_t>(static_cast<uint32_t>(imm) << 1);	// Type-casting due to compiler shenanigans...
}

#endif	// SHIFTLEFT1_INCLUDED