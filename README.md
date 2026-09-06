# Single-Cycle RISC-V Processor Simulator
**Jerry Huang**, 2026. *Inspired by UCLA's EC ENGR M116C Computer Systems Architecture class's Fall 2025 offering taught by Professor Nader Sehatbakhsh.*

## Overview
This is a **cycle-accurate simulator** of a **single-cycle processor** (CPU) using the **32-bit** version of the **RISC-V** instruction set architecture (or at least a small subset of it) written in **C++**. The architecture is based on the Hennessy & Patterson RISC-V block diagram and the code attempts to mirror it as closely as possible; each block is discretely implemented as a module in the form of a class or function.

So far, the simulator supports the following 13 instructions:
- **Arithmetic:** ADDI, LUI, ORI, SLTIU, SRA, SUB, AND
- **Memory access:** LBU, LW, SH, SW
- **Control flow:** BNE, JALR

## File Structure
`cpusim.cpp` contains `main()` and is the "simulator." It parses an instructions file and loads it into a vector, instantiates a `CPU` class object, copies the vector into the instruction memory, and runs the program to completion (or until a maximum number of cycles is hit). Each "cycle" while running the program corresponds to one clock cycle. During each cycle, it also prints the cycle number, the instruction address the program counter is pointing to, the instruction being executed, and that instruction's translation into RISC-V assembly. Finally, it prints out the values stored in registers a0 and a1 at the end of the program.

`CPU.cpp`/`CPU.h` model a processor core's datapath and controller. It instantiates a program counter, register file, instruction memory, and data memory, as well as `CycleState`, a struct containing control signals and other signals that persist between phases in the instruction cycle. It also declares functions for each phase in the instruction cycle: `fetch()`, `decode()`, `execute()`, `memAccess()`, and `writeBack()`.

The datapath consists of the following files:
- `memory.cpp`/`memory.h` contain the instruction memory (4 KB) and data memory (128 KB).  
- `RegFile.cpp`/`RegFile.h` contain the 32-entry register file.  
- `ALU.cpp`/`ALU.h` implements the arithmetic logic unit (ALU) as a function of return type `ALUOutput` containing a `result` and a `zero` flag.  
- `mux.h` implements a 2:1 and 4:1 multiplexer (MUX) as functions.  
- `immGen.h` implements the immediate generator as a function.  
- `shiftLeft1.h` implements the left shifter for branching instructions.  

The controller consists of the following files:
- `control.cpp`/`control.h` implement the main control unit and ALU control unit as functions.

## Running the Simulator
You can try out the simulator for yourself against an instructions file containing RISC-V instructions in hex format, one byte per line. A couple of sample traces (credit to Prof. Sehatbakhsh) are provided in [`traces`](./traces) folder. The files with names starting with `instMem-` contain the actual instructions in hex format; the correspondingly-named files without that prefix contain the disassembled assembly code, along with the expected final a0/a1 values where provided.

The steps to run the simulator are as follows:
1. Compile the simulator:  
`g++ -std=c++17 -Wall -o cpusim *.cpp`
2. Run the executable against a trace file:  
`./cpusim <trace file>`  
or, on Windows:  
`.\cpusim.exe <trace file>`

Enjoy!