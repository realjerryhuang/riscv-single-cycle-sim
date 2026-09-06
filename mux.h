#ifndef MUX_INCLUDED
#define MUX_INCLUDED

#include <cstdint>

template <typename T>
inline T mux2to1(T in1, T in2, bool select)
{
    return select ? in2 : in1;
}

template <typename T>
inline T mux4to1(T in1, T in2, T in3, T in4, uint32_t select)
{
    switch (select) {
        case 0b00:
            return in1;
            break;
        case 0b01:
            return in2;
            break;
        case 0b10:
            return in3;
            break;
        default: // select == 0b11
            return in4;
            break;
    }
}

#endif  // MUX_INCLUDED