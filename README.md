MIPS-simulator
==============

Contains a MIPS simulator and a test bench for all of its instructions

This is a simulator for the MIPS 1 Instruction Set. The cpu is declared in mips_cpu.h and defined in mips_cpu.c. The 
memory of the cpu is declared in mips_mem.h and defined in mips_mem_ram.cpp. The instructions of the MIPS Instruction
Set are declared in mips_cpu_instructions.h and defined in mips_cpu_instructions.cpp

The test bench is declared in mips_test_framework and defined in test_mips.cpp

In order to test the intructions, a instance of cpu is created together with a instance of memory and 32 bits Instruction
codes are directly written into memory. When the test starts, cpu.step() is called for the cpu to advance its pc and 
fetch, decode and execute each of the instruction at the pc address. The program will check if the result is wanted or not
after the execution of each instruction and in the end, the program will list what percentage of the instructions are
correctly executed.
