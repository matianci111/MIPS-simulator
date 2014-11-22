
//
//  test_mips.c
//  ComArg1
//
//  Created by Tianci on 20/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//
#include "mips.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

uint32_t byteswap1(uint32_t memory);
void test(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t setpc, uint32_t dest, uint32_t destt, int type, uint32_t HI, uint32_t LO, mips_mem_h mem, int exception);
void btest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t resultr1, uint32_t result1, uint32_t resultr2, uint32_t result2, uint32_t mem1, uint32_t mem2, int offset, bool success, int type, int exception, mips_mem_h mem);
void mdtest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t memo1, uint32_t memo2, uint32_t setpc, uint32_t dest1, uint32_t dest2, int type, uint32_t HI, uint32_t LO, mips_mem_h mem);
void lstest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t &address, int length, int mem1length, uint32_t memo, uint32_t mem1, uint32_t mem1add, uint32_t dest, uint32_t destt, int type, int exception, mips_mem_h mem);
void swtest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t resultadd, uint32_t result, int type, mips_mem_h mem, int exception);

int main(int argc, const char * argv[])
{
    uint32_t add=0;
    mips_mem_h mem=mips_mem_create_ram(
                                       1<<20,
                                       4
                                       );
    
	mips_cpu_h res=mips_cpu_create(mem);
 	mips_test_begin_suite();
    srand(time(NULL));
    
    for(int i=0;i<100;i++){
        int exception = 1;
        uint32_t r1 = rand() % 0x7fffffff + 1;
        uint32_t r2 = rand() % 0x7fffffff + 1;
        uint32_t answer = r1 + r2;
        int32_t e = (int32_t)r1;
        int32_t f = (int32_t)r2;
        int32_t g = e + f;
        if((e<0 && f<0 && g >=0) || (e>0 && f>0 && g<=0)){
            exception = 0;
        }
        else exception = 1;
        test(res, "ADD", 8, r1, 9, r2, add, 4, 0x1095020, 0, 10, answer, 0, 0, 0, mem, exception);
    }
    test(res, "ADD", 8, 0x7fffffff, 9, 0x7fffffff, add, 4, 0x1095020, 0, 10, 0x7fffffff*2, 0, 0, 0, mem, 0);
    test(res, "ADD", 8, -0x7fffffff, 9, -0x7fffffff, add, 4, 0x1095020, 0, 10, -0x7fffffff*2, 0, 0, 0, mem, 0);
    
    for(int i=0;i<100;i++){
        int exception = 1;
        uint32_t r1 = rand() % 0xffffffff + 1;
        uint16_t r3;
        int16_t r4;
        r3 = (uint16_t)(rand() % 0xffff + 1);
        r4 = (int16_t)r3;
        uint32_t answer = r1 + r4;
        r3 = r3 & 0x0000ffff;
        int32_t e = (int32_t)r1;
        int32_t g;
        g = e+r4;
        if((e<0 && r4<0 && g >=0) || (e>0 && r4>0 && g<=0)){
            exception = 0;
        }
        else exception = 1;
        test(res, "ADDI", 8, r1, 11, 0, add, 4, 0x210A0000|r3, 0, 10, answer, 2, 0, 0, mem, exception);
    }
    
    for(int i=0;i<100;i++){
        uint32_t r1 = rand() % 10000 + -5000;
        uint16_t r3;
        int16_t r4;
        r3 = (uint16_t)(rand() % 10000 + -5000);
        r4 = (int16_t)r3;
        uint32_t answer = r1 + r4;
        r3 = r3 & 0x0000ffff;
        test(res, "ADDIU", 8, r1, 11, 0, add, 4, 0x250A0000|r3, 0, 10, answer, 2, 0, 0, mem, 1 );
    }
    for(int i=0;i<100;i++){
        uint32_t r1 = rand() % 10000 + -5000;
        uint32_t r2 = rand() % 10000 + -5000;
        uint32_t answer = r1 + r2;
        test(res, "ADDU", 8, r1, 9, r2, add, 4, 0x1095021, 0, 10, answer, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        uint32_t answer1 = r5 & r6;
        test(res, "AND", 8, r5, 9, r6, add, 4, 0x1095024, 0, 10, answer1, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r3 = rand()  % 0x0000ffff + 0;
        uint32_t answer = r5 & r3;
        test(res, "ANDI", 8, r5, 9, r3, add, 4, 0x310A0000|r3, 0, 10, answer, 2, 0, 0, mem,1 );
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        int d1 = (int)r5;
        int d2 = (int)r6;
        uint32_t LO = (uint32_t) d1/d2;
        uint32_t HI = (uint32_t) d1%d2;
        mdtest(res, "DIV", 8, r5, 9, r6, add, 4, 0x109001A, 0x5010, 0x5812, 0, 10, 11, 3, HI, LO, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        uint32_t LO = (uint32_t) r5/r6;
        uint32_t HI = (uint32_t) r5%r6;
        mdtest(res, "DIVU", 8, r5, 9, r6, add, 4, 0x109001B, 0x5010, 0x5812, 0, 10, 11, 3, HI, LO, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        int32_t d1 = (int32_t)r5;
        int32_t d2 = (int32_t)r6;
        int64_t intresult;
        uint64_t result;
        uint32_t HI, LO;
        intresult = (d1*d2);
        result = (uint64_t)intresult;
        LO = (uint32_t)result;
        HI = (uint32_t)(result >> 32);
        mdtest(res, "MULT", 8, r5, 9, r6, add, 4, 0x1090018, 0x5010, 0x5812, 0, 10, 11, 3, HI, LO, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        uint64_t result;
        uint32_t HI, LO;
        result = (r5*r6);
        LO = (uint32_t)result;
        HI = (uint32_t)(result >> 32);
        mdtest(res, "MULTU", 8, r5, 9, r6, add, 4, 0x1090019, 0x5010, 0x5812, 0, 10, 11, 3, HI, LO, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        uint32_t answer1 = r5 | r6;
        test(res, "OR", 8, r5, 9, r6, add, 4, 0x1095025, 0, 10, answer1, 0, 0, 0, mem, 1);
        
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r3 = rand()  % 0x0000ffff + 0;
        uint32_t answer = r5 | r3;
        test(res, "ORI", 8, r5, 9, r3, add, 4, 0x350A0000|r3, 0, 10, answer, 2, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r3 = rand()  % 0x1f + 0;
        uint32_t answer = r5 << r3;
        r3 = r3 << 6;
         test(res, "SLL", 8, 1, 9, r5, add, 4, 0x1095000|r3, 0, 10, answer, 4, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r3 = rand()  % 0xffffffff + 0;
        uint32_t answer = r5 << r3;
         test(res, "SLLV", 8, r3, 9, r5, add, 4, 0x1095104, 0, 10, answer, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r6 = rand()  % 0xffffffff + 0;
        int32_t rr5 = (int32_t)r5;
        int32_t rr6 = (int32_t)r6;
        uint32_t answer = 0;
        if(rr5<rr6){
            answer = 1;
        }
        else answer = 0;
        test(res, "SLT", 8, r5, 9, r6, add, 4, 0x109502A, 0, 10, answer, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint16_t r6 = rand()  % 0xffff + 0;
        int32_t rr5 = (int32_t)r5;
        int16_t rr6 = (int16_t)r6;
        uint32_t answer = 0;
        if(rr5<rr6){
            answer = 1;
        }
        else answer = 0;
        test(res, "SLTI", 8, r5, 9, 1, add, 4, 0x290A0000|r6, 0, 10, answer, 2, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint16_t r6 = rand()  % 0xffff + 0;
        int32_t rr5 = (int32_t)r6;
        uint32_t rr6 = (int32_t)rr5;
        uint32_t answer = 0;
        if(r5<rr6){
            answer = 1;
        }
        else answer = 0;
        test(res, "SLTIU", 8, r5, 9, 1, add, 4, 0x2D0A0000|r6, 0, 10, answer, 2, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r6 = rand()  % 0xffffffff + 0;
        uint32_t answer = 0;
        if(r5<r6){
            answer = 1;
        }
        else answer = 0;
        test(res, "SLTU", 8, r5, 9, r6, add, 4, 0x109502B, 0, 10, answer, 0, 0, 0, mem, 1);

    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r6 = rand()  % 0x1f + 0;
        int32_t rr5 = (int32_t)r5;
        rr5 = rr5 >> r6;
        r6 = (r6 << 6)|3;
        uint32_t answer = (uint32_t)rr5;
        test(res, "SRA", 8, 1, 9, r5, add, 4, 0x1095003|r6, 0, 10, answer, 4, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r6 = rand()  % 0x1f + 0;
        uint32_t rr5 = r5 >> r6;
        r6 = (r6 << 6)|3;
        test(res, "SRL", 8, 1, 9, r5, add, 4, 0x1095002|r6, 0, 10, rr5, 4, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r6 = rand()  % 0xffffffff + 0;
        uint32_t rr6 = r6&31;
        uint32_t rr5 = r5 >> rr6;
        
        test(res, "SRLV", 8, r6, 9, r5, add, 4, 0x1095106, 0, 10, rr5, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        int exception;
        uint32_t r1 = rand() % 0xffffffff+ 1;
        uint32_t r2 = rand() % 0xffffffff+ 1;
        uint32_t answer = r1 - r2;
        int32_t e, f, g;
        e = (int32_t)r1;
        f = (int32_t)r2;
        g = e + f;
        if((e<0 && f>0 && g <=0) || (e>0 && f<0 && g>=0)){
            exception = 0;
        }
        else exception = 1;
        test(res, "SUB", 8, r1, 9, r2, add, 4, 0x1095022, 0, 10, answer, 0, 0, 0, mem, exception);
    }
    for(int i=0;i<100;i++){
        uint32_t r1 = rand() % 0xffffffff+ 1;
        uint32_t r2 = rand() % 0xffffffff+ 1;
        uint32_t answer = r1 - r2;
        test(res, "SUBU", 8, r1, 9, r2, add, 4, 0x1095023, 0, 10, answer, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand() % 0xffffffff + 0;
        uint32_t r6 = rand() % 0xffffffff + 0;
        uint32_t answer1 = r5 ^ r6;
        test(res, "XOR", 8, r5, 9, r6, add, 4, 0x1095026, 0, 10, answer1, 0, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r5 = rand()  % 0xffffffff + 0;
        uint32_t r3 = rand()  % 0x0000ffff + 0;
        uint32_t answer = r5 ^ r3;
        test(res, "XORI", 8, r5, 9, r3, add, 4, 0x390A0000|r3, 0, 10, answer, 2, 0, 0, mem, 1);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2;
        int a2 = rand() % 2;
        bool successbranch = (a1 == a2);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BEQ", 8, a1, 9, a2, add, 4, 0x11090000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 >= 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BGEZ", 8, a1, 9, a2, add, 4, 0x5010000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 >= 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BGEZAL", 8, a1, 9, a2, add, 4, 0x5110000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 1, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2;
        int a2 = rand() % 2;
        bool successbranch = (a1 > 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
         btest(res, "BGTZ", 8, a1, 9, a2, add, 4, 0x1D000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 <= 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BLEZ", 8, a1, 9, a2, add, 4, 0x19000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 < 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
         btest(res, "BLTZ", 8, a1, 9, a2, add, 4, 0x5000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 < 0);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BLTZAL", 8, a1, 9, a2, add, 4, 0x5100000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 1, exception, mem);

    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while((((int16_t)r3)<<2)<0  && (((int16_t)r3)<<2)>-(int32_t)add){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
           int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        bool successbranch = (a1 != a2);
        uint32_t addr;
        int exception = 0;
        mips_cpu_get_pc(res, &addr);
        if(((((int16_t)r3)<<2)+(int32_t)addr)<0){
            exception = 1;
        }
        
        btest(res, "BNE", 8, a1, 9, a2, add, 4, 0x15090000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3<<2, successbranch, 0, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x3fffffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3)<<2)) > (1<<20)){
            r3 = rand()  % (0x3fffffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        int exception = 0;
        if(((((int32_t)r3)<<2))<0){
            exception = 1;
        }
        
        btest(res, "J", 8, a1, 9, a2, add, 4, 0x8000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3, true, 3, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3)<<2)) > (1<<20)){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        int exception = 0;
        if(((((int32_t)r3)<<2))<0){
            exception = 1;
        }
        
        btest(res, "J", 8, a1, 9, a2, add, 4, 0x8000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3, true, 3, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x3fffffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3)<<2)) > (1<<20)){
            r3 = rand()  % (0x3fffffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        int exception = 0;
        if(((((int32_t)r3)<<2))<0){
            exception = 1;
        }
        
        btest(res, "JAL", 8, a1, 9, a2, add, 4, 0xC000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826,  0x1096024, r3, true, 4, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3)<<2)) > (1<<20)){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int a1= rand() % 2-1;
        int a2 = rand() % 2-1;
        int exception = 0;
        if(((((int32_t)r3)<<2))<0){
            exception = 1;
        }
        
        btest(res, "JAL", 8, a1, 9, a2, add, 4, 0xC000000|r3, 11, a1^a2, 12, a1&a2, 0x1095826, 0x1096024, r3, true, 4, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0xffffffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3))) > (1<<20)){
            r3 = rand()  % (0xffffffff-(1024<<1)) + (1024<<1);
        }
        int exception = 0;
        if(((((int32_t)r3)))<0){
            exception = 1;
        }
        else if((r3&3) != 0){
            exception = 2;
        }
        
        btest(res, "JR", 8, r3, 9, r3, add, 4, 0x1000008, 11, r3^r3, 12, r3&r3, 0x1095826, 0x1096024, r3, true, 5, exception, mem);
    }
    for(int i=0;i<100;i++){
        uint32_t r3 = rand()  % (0xffffffff-(1024<<1)) + (1024<<1);
        while(((((int32_t)r3))) > (1<<20) || ((r3&3) != 0)){
            r3 = rand()  % (0xffffffff-(1024<<1)) + (1024<<1);
        }
        int exception = 0;
        if(((((int32_t)r3)))<0){
            exception = 1;
        }
        if((r3&3) != 0){
            exception = 2;
        }
        
        btest(res, "JR", 8, r3, 9, r3, add, 4, 0x1000008, 11, r3^r3, 12, r3&r3, 0x1095826, 0x1096024, r3, true, 5, exception, mem);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        uint32_t realr3 = r3 - (r3%4);
        int8_t result = 0;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want
        switch (r3%4) {
            case 0:
                result = (int8_t)(inthatmem >> 24);
                break;
            case 1:
                result = (int8_t)((inthatmem & 0x00ff0000) >> 16);
                break;
            case 2:
                result = (int8_t)((inthatmem & 0x0000ff00) >> 8);
                break;
            case 3:
                result = (int8_t)((inthatmem &0xff));
                break;
            default:
                break;
        }
        int32_t ite = (int32_t)result;
        uint32_t finalresult = (uint32_t)ite;
        lstest(res, "LB", 8, 0, add, 4, 4, 0x810A0000|r3, inthatmem, realr3, 10, finalresult, 0, exception, mem);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        uint32_t realr3 = r3 - (r3%4);
        uint32_t result = 0;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want
        switch (r3%4) {
            case 0:
                result = inthatmem >> 24;
                break;
            case 1:
                result = (inthatmem & 0x00ff0000) >> 16;
                break;
            case 2:
                result = (inthatmem & 0x0000ff00) >> 8;
                break;
            case 3:
                result = (inthatmem &0xff);
                break;
            default:
                break;
        }
        
        lstest(res, "LBU", 8, 0, add, 4, 4, 0x910A0000|r3, inthatmem, realr3, 10, result, 0, exception, mem);
    }
    
    for(int i=0;i<100;i++){
        uint16_t r5 = rand()  % 0x0000ffff + 0;
        uint32_t answer = r5 << 16;
        test(res, "LUI", 8, 4, 11, 1, add, 4, 0x3D0A0000| r5, 0, 10, answer, 2, 0, 0, mem, 1);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        else if((r3&3)!=0){
            exception = 2;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want
        
        
        lstest(res, "LW", 8, 0, add, 4, 4, 0x8D0A0000|r3, inthatmem, r3, 10, inthatmem, 0, exception, mem);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        while(((((int16_t)r3)<<2)) > (1<<20) || ((((int16_t)r3)<<2)) < 0 || (r3&3)!=0){
            r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        }
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        else if((r3&3)!=0){
            exception = 2;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want

        
        lstest(res, "LW", 8, 0, add, 4, 4, 0x8D0A0000|r3, inthatmem, r3, 10, inthatmem, 0, exception, mem);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        uint32_t r5 = rand() % 0xffffffff +1;
        mips_cpu_set_register(res, 10, r5);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        uint32_t realr3 = r3 - (r3%4);
        uint32_t result = 0;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want
        switch (r3%4) {
            case 0:
                result = inthatmem;
                break;
            case 1:
                result = (r5&0x000000ff) | (inthatmem << 8);
                break;
            case 2:
                result = (r5&0x0000ffff) | (inthatmem << 16);
                break;
            case 3:
                result = (r5&0x00ffffff) | (inthatmem << 24);
                break;
            default:
                break;
        }
        lstest(res, "LWL", 8, 0, add, 4, 4, 0x890A0000|r3, inthatmem, realr3, 10, result, 0, exception, mem);
    }
    for(int i=0;i<50;i++){
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        uint32_t r5 = rand() % 0xffffffff +1;
        mips_cpu_set_register(res, 10, r5);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        uint32_t inthatmem = rand() % 0xffffffff +1;
        uint32_t realr3 = r3 - (r3%4);
        uint32_t result = 0;
        //store a memory to an address
        //load a specific byte from that address
        //see if the byte is what we want
        switch (r3%4) {
            case 0:
                result = (r5&0xffffff00) | (inthatmem >> 24);
                break;
            case 1:
                result = (r5&0xffff0000) | (inthatmem >> 16);
                break;
            case 2:
                result = (r5&0xff000000) | (inthatmem >> 8);
                break;
            case 3:
                result = inthatmem;
                break;
            default:
                break;
        }
        lstest(res, "LWR", 8, 0, add, 4, 4, 0x990A0000|r3, inthatmem, realr3, 10, result, 0, exception, mem);
    }
    for(int i = 0; i<100;i++){
        uint32_t indatmem = rand() % 0xffffffff + 1;
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        else if((r3&3)!=0){
            exception = 2;
        }
        swtest(res, "SW", 8, 0, 9, indatmem, add, 4, 0xAD090000|r3, r3, indatmem, 0, mem, exception);
    }
    for(int i = 0; i<100;i++){
        uint32_t indatmem = rand() % 0xffffffff + 1;
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        else if((r3&1)!=0){
            exception = 2;
        }
        uint32_t finalind= indatmem & 0x0000ffff;
        swtest(res, "SH", 8, 0, 9, indatmem, add, 4, 0xA5090000|r3, r3, finalind, 1, mem, exception);
    }
    for(int i = 0; i<100;i++){
        uint32_t indatmem = rand() % 0xffffffff + 1;
        uint16_t r3 = rand()  % (0x0000ffff-(1024<<1)) + (1024<<1);
        int exception = 0;
        if(((int16_t)r3)<0){
            exception = 1;
        }
        uint32_t finalind = indatmem & 0x000000ff;
        swtest(res, "SB", 8, 0, 9, indatmem, add, 4, 0xA1090000|r3, r3, finalind, 2, mem, exception);
    }
    
    
    mips_test_end_suite();
	mips_cpu_free(res);
	mips_mem_free(mem);
    
	return 0;
    return 0;
}

uint32_t byteswap1(uint32_t memory){
    return   memory<<24| (memory & 0x0000ff00)<<8| (memory & 0x00ff0000)>>8| memory >> 24;
}

void test(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t setpc, uint32_t dest, uint32_t destt, int type, uint32_t HI, uint32_t LO, mips_mem_h mem, int exception){
    int testId=mips_test_begin_test(a);
	int passed=0;
    mips_error err = mips_Success;
    if(type==0 || type==3){
        err = mips_cpu_set_register(res, r1, rr1);
        if(err==0)
            err = mips_cpu_set_register(res, r2, rr2);
    }
    
    if(type==2){
        err = mips_cpu_set_register(res, r1, rr1);
    }
    
    if(type==4){
        err = mips_cpu_set_register(res, r2, rr2);
    }

    uint32_t memory = memo;
    memory = byteswap1(memory);
    if(err == 0)
        err = mips_mem_write(mem, address, length, (uint8_t*)&memory);

    
    
	if(err==0){
		err=mips_cpu_step(res);
    }
    

    
	uint32_t got = 0;
    
	if(err==0)
		err = (mips_error)(err | mips_cpu_get_register(res, dest, &got));

    
    if(type!=3)
        passed = (err == mips_Success) && (got==destt);
    
    if(exception == 0){
        passed = (err == mips_ExceptionArithmeticOverflow);
    }
	mips_test_end_test(testId, passed, NULL);
    address=address+4;
}

void mdtest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t memo1, uint32_t memo2, uint32_t setpc, uint32_t dest1, uint32_t dest2, int type, uint32_t HI, uint32_t LO, mips_mem_h mem){

    int testId=mips_test_begin_test(a);
    mips_error err;

        err = mips_cpu_set_register(res, r1, rr1);
        if(err==0)
            err = mips_cpu_set_register(res, r2, rr2);

    
    uint32_t memory = memo;
    memory = byteswap1(memory);
    memo1 = byteswap1(memo1);
    memo2 = byteswap1(memo2);
    
    if(err==0)
        err = mips_mem_write(mem, address, length, (uint8_t*)&memory);
    if(err==0)
        err = mips_mem_write(mem, address+4, length, (uint8_t*)&memo1);
    if(err==0)
        err = mips_mem_write(mem, address+8, length, (uint8_t*)&memo2);
    //write arithmetic to memory 1
    //write mfhi to memory 2

    
	if(err==0){
		err=mips_cpu_step(res);
    }
    
    if(err==0){
		err=mips_cpu_step(res);
    }
    if(err==0){
		err=mips_cpu_step(res);
    }
    uint32_t result1, result2;
    if(err==0){
		err=mips_cpu_get_register(res, dest1, &result1);
    }
    if(err==0){
		err=mips_cpu_get_register(res, dest2, &result2);
    }
    
    
    if((result1 == HI) && (result2 == LO)){
    
        mips_test_end_test(testId, 1, NULL);
        testId = mips_test_begin_test("MFHI");
        mips_test_end_test(testId, 1, NULL);
        testId = mips_test_begin_test("MFLO");
        mips_test_end_test(testId, 1, NULL);
    }
    else{
        
        mips_test_end_test(testId, 0, NULL);
        testId = mips_test_begin_test("MFHI");
        mips_test_end_test(testId, 0, NULL);
        testId = mips_test_begin_test("MFLO");
        mips_test_end_test(testId, 0, NULL);
    }
    
    address=address+12;
}

void btest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t resultr1, uint32_t result1, uint32_t resultr2, uint32_t result2, uint32_t mem1, uint32_t mem2, int offset, bool success, int type, int exception, mips_mem_h mem){
    int testId=mips_test_begin_test(a);
    uint32_t jumpidx;
    jumpidx = (uint32_t)offset;
	int passed=0;
    mips_error err;
    mem1 = byteswap1(mem1);
    mem2 = byteswap1(mem2);
    memo = byteswap1(memo);
    uint32_t tmemory = mem1; //XOR 8 and 9, should give 0 at 11
    uint32_t pc=0;
    err = mips_cpu_get_pc(res, &pc);
    err = mips_mem_write(mem, pc+4, 4, (uint8_t*)&tmemory);
    tmemory =mem2; //AND 8 and 9, should give 1 at 12
    
    if((type == 3 || type == 4) && err ==0)
        err = mips_mem_write(mem, (pc & 0xfc000000) | (jumpidx << 2), 4, (uint8_t*)&tmemory);
    else if(type == 5 && err == 0){
        offset = offset - (offset%4);
        err = mips_mem_write(mem, offset, 4, (uint8_t*)&tmemory);
    }
    else{
        if(err==0)
            err = mips_mem_write(mem, pc+4+offset, 4, (uint8_t*)&tmemory);
    }
    
    
    
    if(err==0)
        err = mips_cpu_set_register(res, r1, rr1);
    if(err==0)
        err = mips_cpu_set_register(res, r2, rr2);
    
    uint32_t memory = memo;
    if(err==0)
        err = mips_mem_write(mem, address, length, (uint8_t*)&memory);
    
    
	if(err==0){
		err=mips_cpu_step(res);
    }
    
    if(success && err==0)
        err= mips_cpu_step(res);
    if(err==0)
        err = mips_cpu_get_pc(res, &pc);
    uint32_t a1=0, a2=0, a3=0;
    if(err==0)
        err = mips_cpu_get_register(res, resultr1, &a1);
    if(err==0)
        err = mips_cpu_get_register(res, resultr2, &a2);
    if(err==0)
        err = mips_cpu_get_register(res, 31, &a3);
    
    if((type != 3) && (type != 4)){
        if(success && type == 0){
            passed = (err == mips_Success) && ( pc == address + offset + 8) && (result1 == a1) && (result2 == a2);
        }
        else if(success && type == 1){
            passed = (err == mips_Success) && (pc == address + offset + 8) && (result1 == a1) && (result2 == a2) && (a3 == address + 8);
        }
        else if(success && type == 5){
            passed = (err == mips_Success) && (pc == offset + 4) && (result1 == a1) && (result2 == a2);
        }
        else{
            passed = ((err == mips_Success) )&& (pc == address + 4);
        }
    }
    else{
        if(type == 3){
            passed = (err == mips_Success) && (pc == ((address & 0xf0000000) | (jumpidx << 2))+4) && (result1 == a1) && (result2 == a2);

        }
        else{
            passed = (err == mips_Success) && (pc == ((address & 0xf0000000) | (jumpidx << 2))+4) && (result1 == a1) && (result2 == a2)&&(a3 == address + 8);

        }
    }
    if(exception==1){
        passed = (err == mips_ExceptionInvalidAddress);
    }
    if(exception==2){
        passed = (err == mips_ExceptionInvalidAlignment);
    }
    mips_cpu_set_pc(res, address + 8);

    
	mips_test_end_test(testId, passed, NULL);
    address=address+8;
}

void lstest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t &address, int length, int mem1length, uint32_t memo, uint32_t mem1, uint32_t mem1add, uint32_t dest, uint32_t destt, int type, int exception, mips_mem_h mem){
    int testId=mips_test_begin_test(a);
	int passed=0;
    
    mips_error err;
    
    err = mips_cpu_set_register(res, r1, rr1);
    uint32_t tmem = mem1;
    tmem = byteswap1(tmem);
    err = mips_mem_write(mem, mem1add, mem1length, (uint8_t*)&tmem);
    if(type==1){
        err = mips_mem_write(mem, mem1add+4, mem1length, (uint8_t*)&tmem);
    }
    uint32_t memory = memo;
    memory = byteswap1(memory);
    err = mips_mem_write(mem, address, length, (uint8_t*)&memory);
    
    
	if(err==0){
		err=mips_cpu_step(res);
    }
    

	uint32_t got = 0;
    
	if(err==0)
		err = (mips_error)(err | mips_cpu_get_register(res, dest, &got));
    
    passed = (err == mips_Success) && (got==destt);
    
    if(exception == 1){
        passed = (err == mips_ExceptionInvalidAddress);
    }
    if(exception == 2){
        passed = (err == mips_ExceptionInvalidAlignment);
    }
    
	mips_test_end_test(testId, passed, NULL);
    address= address+4;
}

void swtest(mips_cpu_h res, const char *a, uint32_t r1, uint32_t rr1, uint32_t r2, uint32_t rr2, uint32_t &address, int length, uint32_t memo, uint32_t resultadd, uint32_t result, int type, mips_mem_h mem, int exception){
    int testId=mips_test_begin_test(a);
	int passed=0;
    

        mips_error err = mips_cpu_set_register(res, r1, rr1);
    if(err==0)
        err = mips_cpu_set_register(res, r2, rr2);
    
    uint32_t memory = memo;
    
    memory = byteswap1(memory);
    if(err==0)
        err = mips_mem_write(mem, address, length, (uint8_t*)&memory);
    
    
	if(err==0){
		err=mips_cpu_step(res);
    }
    
	uint32_t got = 0;
    
	if(err==0){
        if(type==0)
            err = mips_mem_read(mem, resultadd, length, (uint8_t*)&got);
        if(type==1||type ==2){
            uint32_t newadd = resultadd - (resultadd %4);
            err = mips_mem_read(mem, newadd, length, (uint8_t*)&got);
        }
    }
    
    got = byteswap1(got);
    if(type == 1){
        switch (resultadd%4) {
            case 0:
                got = got >> 16;
                break;
            case 2:
                got = got & 0x0000ffff;
                break;
            default:
                break;
        }
    }
    if(type ==2){
        switch (resultadd%4) {
            case 0:
                got = got >> 24;
                break;
            case 1:
                got = (got & 0x00ff0000) >> 16;
                break;
            case 2:
                got = (got & 0x0000ff00) >> 8;
                break;
            case 3:
                got = (got & 0x000000ff);
                break;
            default:
                break;
        }
    }
    
    passed = (err == mips_Success) && (got==result);
    
    if(exception == 1){
        passed = (err == mips_ExceptionInvalidAddress);
    }
    if(exception == 2){
        passed = (err == mips_ExceptionInvalidAlignment);
    }
    
	mips_test_end_test(testId, passed, NULL);
    address= address+4;
}