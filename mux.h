#ifndef MUX_INCLUDED
#define MUX_INCLUDED

template <typename T>
inline T mux2to1(T in1, T in2, bool select)
{
    return select ? in2 : in1;
}

#endif  // MUX_INCLUDED