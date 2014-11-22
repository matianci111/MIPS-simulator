//
//  mips_cpu.cpp
//  project
//
//  Created by Tianci on 28/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//

//
//  mips_cpu.c
//  ComArg1
//
//  Created by Tianci on 20/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//
#include "mips.h"
#include "mips_cpu_instructions.h"
#include <stdio.h>
struct mips_cpu_impl{
    uint32_t pc;
    uint32_t pcN;
    uint32_t regs[32];
    uint32_t LO;
    uint32_t HI;
    FILE* dest;
    unsigned debugginglvl;
    
    
    mips_mem_h mem;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem){
    mips_cpu_h res = new struct mips_cpu_impl;
    for(int i = 0; i<32;i++){
        res->regs[i] = 0;
    }
    res->mem = mem;
    res->pc=0;
    res->pcN=4;
    res->LO = 0;
    res->HI = 0;
    return res;
}

void mips_cpu_free(mips_cpu_h state)
{
    if(state){
        free(state);
        state = 0;
    }
}


mips_error mips_cpu_reset(mips_cpu_h state){
    for(int i = 0; i<32;i++){
        state->regs[i] = 0;
    }
    state->LO = 0;
    state->HI = 0;
    return mips_Success;
}


mips_error mips_cpu_get_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t *value		//!< Where to write the value to
){
    if(state==0)
		return mips_ErrorInvalidHandle;
	if(index>=32)
		return mips_ErrorInvalidArgument;
	if(value==0)
		return mips_ErrorInvalidArgument;
    *value = state->regs[index];
    return mips_Success;
}


mips_error mips_cpu_set_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t value		//!< New value to write into register file
){
    if(state==0)
		return mips_ErrorInvalidHandle;
	if(index>=32)
		return mips_ErrorInvalidArgument;
    if(index==0)
        return mips_ErrorInvalidArgument;
    state->regs[index] = value;
    return mips_Success;
}


mips_error mips_cpu_set_pc(
                           mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                           uint32_t pc			//!< Address of the next instruction to exectute.
){
    state->pc = pc;
    state->pcN = pc +4;
    return mips_Success;
}


mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc){
    *pc = state->pc;
    return mips_Success;
}


mips_error mips_cpu_step(mips_cpu_h state){

    mips_error err;
    uint32_t pc=state->pc;
    uint32_t instruction, Rtmp, Jtmp;
    uint32_t rmask = 0;
    uint32_t jmask = 1;
	if(state==0)
		return mips_ErrorInvalidHandle;
    
	//TODO: Here is where the magic happens
	// - Fetch the instruction from memory (mips_mem_read)
	// - Decode the instruction (is it R, I, J)?
	// - Execute the instruction (do maths, access memory, ...)
	// - Writeback the results (update registers, advance pc)
    err = mips_mem_read(state->mem, pc, 4, (uint8_t*)&instruction);
    instruction = byteswap(instruction);

    
    Rtmp = instruction >> 26;
    Jtmp = instruction >> 27;
    if((Rtmp ^ rmask) == 0){
        //R type
        uint8_t Rinstr = instruction & 63;
        uint8_t S1 = instruction >> 21;
        uint8_t S2 = instruction >> 16;
        S2 = S2 & 31;
        uint32_t dest = instruction >> 11;
        dest = dest & 31;
        uint32_t shift = instruction >> 6;
        shift = shift & 31;
        switch (Rinstr) {
            case 0:
                err = sll(S2, dest, shift, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 2:
                err = srl(S2, dest, shift, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 3:
                err = sra(S2, dest, shift, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 4:
                err = sllv(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 6:
                err = srlv(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 8:
                err = jr(S1, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 32:
                err = add(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 33:
                err = addu(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 34:
                err = sub(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 35:
                err = subu(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 36:
                err = andd(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 37:
                err = orr(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 38:
                err = xorr(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 42:
                err = slt(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 43:
                err = sltu(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 24:
                err = multiply(S1, S2, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 25:
                err = multiplyunsigned(S1, S2, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 26:
                err = division(S1, S2, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 27:
                err = divisionu(S1, S2, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 16:
                err = mfhi(dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 18:
                err = mflo(dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
                
            default:
                break;
        }
        
    }
    else if((Jtmp ^ jmask) == 0){
        //J type
        uint32_t memadd = instruction & 67108863;
        uint8_t Jinstr = instruction >> 26;
        switch (Jinstr) {
            case 2:
                err = jump(memadd, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 3:
                err = jal(memadd, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
                
            default:
                break;
        }
        
    }
    else{
        //I type
        uint16_t imme = instruction & 65535;
        uint8_t dest = instruction >> 16;
        dest = dest & 31;
        uint8_t src = instruction >> 21;
        src = src & 31;
        uint8_t Iinstr = instruction >> 26;
     
        
        switch (Iinstr) {
            case 1:
                switch(dest){
                    case 0:
                        err = bltz(src, (int16_t)imme, state);
                        state->pc = state->pcN;
                        state->pcN = state->pc+4;
                        break;
                    case 1:
                        err = bgez(src, (int16_t)imme, state);
                        state->pc = state->pcN;
                        state->pcN = state->pc+4;
                        break;
                    case 16:
                        err = bltzal(src, (int16_t)imme, state);
                        state->pc = state->pcN;
                        state->pcN = state->pc+4;
                        break;
                    case 17:
                        err = bgezal(src, (int16_t)imme, state);
                        state->pc = state->pcN;
                        state->pcN = state->pc+4;
                        break;
                }
                break;
            case 4:
                err = beq(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 5:
                err = bne(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 6:
                err = blez(src, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 7:
                err = bgtz(src, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 8:
                err = addi(src, (int16_t)imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 9:
                err = addiu(src, imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 10:
                err = slti(src, (int64_t)imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 11:
                err = sltiu(src, imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 12:
                err = andi(src, imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 13:
                err = ori(src, imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 14:
                err = xori(src, imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 15:
                err = lui(dest, imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 32:
                err = lb(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 34:
                err = lwl(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 35:
                err = lw(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 36:
                err = lbu(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 38:
                err = lwr(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 43:
                err = sw(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 41:
                err = shw(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            case 40:
                err = sb(src, dest, (int16_t)imme, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
            default:
                break;
        }
        
        
    }
    
	return err;
}


mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
{
    state->debugginglvl = level;
    state->dest = dest;
    return mips_Success;
}

