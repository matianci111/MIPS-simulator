//
//  mips_cpu_instructions.h
//  project
//
//  Created by Tianci on 28/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//

#ifndef __project__mips_cpu_instructions__
#define __project__mips_cpu_instructions__

#include <iostream>
#include "mips_cpu.h"
mips_error add(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res);
mips_error addi(unsigned r1, int16_t imme, unsigned d, mips_cpu_h res);
mips_error addiu(unsigned r1, int16_t imme, unsigned d, mips_cpu_h res);
mips_error addu(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res);
mips_error andd(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res);
mips_error andi(unsigned r1, uint16_t imme, unsigned d, mips_cpu_h res);
mips_error beq(unsigned r1, unsigned r2, int16_t offset, mips_cpu_h res);
mips_error bgez(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error bgezal(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error bgtz(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error blez(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error bltz(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error bltzal(unsigned r1, int16_t offset, mips_cpu_h res);
mips_error bne(unsigned r1, unsigned r2, int16_t offset, mips_cpu_h res);
mips_error division(unsigned r1, unsigned r2, mips_cpu_h res);
mips_error divisionu(unsigned r1, unsigned r2, mips_cpu_h res);
mips_error jump(uint32_t dest, mips_cpu_h res);
mips_error jal(uint32_t dest, mips_cpu_h res);
mips_error jr(unsigned r1, mips_cpu_h res);
mips_error lb(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error lbu(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error lui(unsigned r2, uint16_t imme, mips_cpu_h res);
mips_error lw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error lwl(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error lwr(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error mfhi(unsigned dest, mips_cpu_h res);
mips_error mflo(unsigned dest, mips_cpu_h res);
mips_error multiply(unsigned r1, unsigned r2, mips_cpu_h res);
mips_error multiplyunsigned(unsigned r1, unsigned r2, mips_cpu_h res);
mips_error orr(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error ori(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res);
mips_error sb(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error shw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error sll(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res);
mips_error sllv(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error slt(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error slti(unsigned r1, int16_t imme, unsigned dest, mips_cpu_h res);
mips_error sltiu(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res);
mips_error sltu(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error sra(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res);
mips_error srl(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res);
mips_error srlv(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error sub(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error subu(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error sw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res);
mips_error xorr(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res);
mips_error xori(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res);
uint32_t byteswap(uint32_t memory);
#endif
