//
//  test_mips.c
//  ComArg1
//
//  Created by Tianci on 20/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//
#include "mips.h"
#include "instructions.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
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
void tadd();


int main(int argc, const char * argv[])
{
    mips_mem_h mem=mips_mem_create_ram(
                                       1<<20,
                                       4
                                       );
    
	mips_cpu_h res=mips_cpu_create(mem);
    
	mips_test_begin_suite();
    trtyppe(res, "add", 8, -1, 9, -1, 0, 4, 0b00000001000010010101000000100000, 0, 10, -2);

    
	mips_test_end_suite();
    
    
    
	mips_cpu_free(res);
	mips_mem_free(mem);
    
	return 0;
    return 0;
}

void trtype(mips_cpu_h res, string name, ){
    int testId=mips_test_begin_test("add");
	int passed=0;
	mips_error err = mips_cpu_set_register(res, 8, -1);
	if(err==0)
		err = mips_cpu_set_register(res, 9, -1);
    uint32_t memory = 0b00000001000010010101000000100000;
    err = mips_mem_write(res->mem, 0, 4, (uint8_t*)&memory);
    mips_cpu_set_pc(res, 0);
    
    
	if(err==0)
		err=mips_cpu_step(res);
    
	uint32_t got = 0;
	if(err==0)
		err = (mips_error)(err | mips_cpu_get_register(res, 10, &got));

	passed = (err == mips_Success) && (got==-2);
    
	mips_test_end_test(testId, passed, NULL);
}