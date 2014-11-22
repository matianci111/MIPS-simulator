//
//  mips_cpu.c
//  ComArg1
//
//  Created by Tianci on 20/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//
#include "mips.h"
#include "instructions.h"
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
    mips_cpu_h res = malloc (sizeof(struct mips_cpu_impl));
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
        free(state->dest);
        state->dest = 0;
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
            case 32:
                err = add(S1, S2, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
         //other functions
                
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
                //jump type
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
            case 8:
                addi(src, (int16_t)imme, dest, state);
                state->pc = state->pcN;
                state->pcN = state->pc+4;
                break;
        //other functions
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
