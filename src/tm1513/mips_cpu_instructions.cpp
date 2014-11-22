//
//  mips_cpu_instructions.cpp
//  project
//
//  Created by Tianci on 28/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//

#include "mips_cpu_instructions.h"
//
//  instructions.c
//  ComArg1
//
//  Created by Tianci on 21/10/2014.
//  Copyright (c) 2014 Tianci. All rights reserved.
//

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

mips_error add(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    int32_t e, f, g;
    err = mips_cpu_get_register(res, r1, &a);
    if(err==0)
        err = mips_cpu_get_register(res, r2, &b);

    e = (int32_t)a;
    f = (int32_t)b;
    g = e + f;
    c = a + b;
    if((e<0 && f<0 && g > 0) || (e>0 && f>0 && g<0)){
        if(err==0)
            err = mips_ExceptionArithmeticOverflow;
            printf("detected");
    }
    else{
        if(err==0)
            err = mips_cpu_set_register(res, d, c);
    }
    return err;
}

mips_error addi(unsigned r1, int16_t imme, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t c;
    int32_t e, f, g;
    err = mips_cpu_get_register(res, r1, &a);
    c = a + imme;
    e = (int32_t)a;
    g = e+imme;
    if((e>0 && imme>0 && g <= 0) || (e<0 && imme<0 && g >= 0)){
        if(err==0)
            err = mips_ExceptionArithmeticOverflow;
    }
    else{
        if(err==0)
            err = mips_cpu_set_register(res, d, c);
    }
    return err;
}

mips_error addiu(unsigned r1, int16_t imme, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t c;
    err = mips_cpu_get_register(res, r1, &a);
    c = a + imme;
    if(err==0)
        err = mips_cpu_set_register(res, d, c);
    return err;
}

mips_error addu(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    err = mips_cpu_get_register(res, r1, &a);
    if(err==0)
        err = mips_cpu_get_register(res, r2, &b);
    c = a + b;
    if(err==0)
        err = mips_cpu_set_register(res, d, c);
    return err;
}

mips_error andd(unsigned r1, unsigned r2, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    err=mips_cpu_get_register(res, r1, &a);
    if(err==0)
        err=mips_cpu_get_register(res, r2, &b);
    c = a & b;
    err = mips_cpu_set_register(res, d, c);
    return err;
}

mips_error andi(unsigned r1, uint16_t imme, unsigned d, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t c;
    err = mips_cpu_get_register(res, r1, &a);
    c = a & imme;
    if(err==0)
        err = mips_cpu_set_register(res, d, c);
    return err;
}

mips_error beq(unsigned r1, unsigned r2, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t addr;
    err = mips_cpu_get_register(res, r1, &a);
    if(err==0)
        err = mips_cpu_get_register(res, r2, &b);
    if(err==0)
        err = mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
   if(((tmp+(offset<<2)) >= 0) ){
       if(a == b){
            res->pc=res->pcN;
           if(err==0)
               err = mips_cpu_step(res);//pc = pcN both at the delay slot
           if(err==0)
               err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bgez(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err = mips_cpu_get_register(res, r1, &a);
    if(err==0)
        err = mips_cpu_get_pc(res, &addr);
     int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a >= 0){
            res->pc=res->pcN;
            mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bgezal(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err=mips_cpu_get_register(res, r1, &a);
    err=mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a >= 0){
            res->regs[31] = addr + 8;
            res->pc=res->pcN;
            mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bgtz(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a > 0){
            res->pc=res->pcN;
            mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error blez(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a <= 0){
            res->pc=res->pcN;
            mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bltz(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a < 0){
            res->pc=res->pcN;
            err = mips_cpu_step(res);//pc = pcN both at the delay slot
            if(err == 0)
                err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bltzal(unsigned r1, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t addr;
    err=mips_cpu_get_register(res, r1, &a);
    err=mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if((int32_t)a < 0){
            res->regs[31] = addr + 8;
            res->pc=res->pcN;
            err = mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error bne(unsigned r1, unsigned r2, int16_t offset, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t addr;
    
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    err = mips_cpu_get_pc(res, &addr);
    int32_t tmp = (int32_t)addr;
    if(((tmp+(offset<<2)) >= 0) ){
        if(a!=b){
            res->pc=res->pcN;
            err = mips_cpu_step(res);//pc = pcN both at the delay slot
            err = mips_cpu_set_pc(res, addr+(offset<<2));
            res->pcN = res->pc+4;
        }
    }
    else{
        return mips_ExceptionInvalidAddress;
        
    }
    return err;
}

mips_error division(unsigned r1, unsigned r2, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    int tmpa, tmpb;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);

    tmpa = (int)a;
    tmpb = (int)b;
    res->LO = (uint32_t) (tmpa / tmpb);
    res->HI = (uint32_t) (tmpa % tmpb);
    return err;
}

mips_error divisionu(unsigned r1, unsigned r2, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    res->LO = a / b;
    res->HI = a % b;
    
    return err;
}

mips_error jump(uint32_t dest, mips_cpu_h res){
    mips_error err;
    if(((((int32_t)dest<<2)) >= 0)){
        res->pc=res->pcN;
        err = mips_cpu_step(res);//pc = pcN both at the delay slot
        res->pcN =  ((res->pc & 0xf0000000) | (dest << 2));
    }
    else{
        return mips_ExceptionInvalidAddress;
    }
    return err;
}

mips_error jal(uint32_t dest, mips_cpu_h res){
    res->regs[31] = res->pc + 8;
    mips_error err;
    if(((((int32_t)dest<<2)) >= 0)){
        res->pc=res->pcN;
        err = mips_cpu_step(res);//pc = pcN both at the delay slot
        res->pcN =  ((res->pc & 0xf0000000) | (dest << 2));
    }
    else{
        return mips_ExceptionInvalidAddress;
    }
    return err;
}

mips_error jr(unsigned r1, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    err = mips_cpu_get_register(res, r1, &a);
    if(((((int32_t)a<<2)) >= 0) && ((a&3) == 0)){
        res->pc = res->pcN;
        err = mips_cpu_step(res);
        res->pcN = a;
    }
    else if(((((int32_t)a<<2)) < 0)){
        return mips_ExceptionInvalidAddress;
    }
    else{
        return mips_ExceptionInvalidAlignment;
    }
    
    return err;
}

mips_error lb(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t b;
    uint32_t dataout;
    uint8_t dataa[4];
    int8_t datatem=0;
    uint32_t newb;
    int32_t intre;
    uint32_t finalre;
    err = mips_cpu_get_register(res, r1, &a);
    b = a + imme;
    if((int32_t)b >= 0 && (int32_t)b < (1<<20)){
        newb = b - (b % 4);
        err = mips_mem_read(res->mem, newb, 4, (uint8_t*)&dataout);
        for(int i=0;i<4;i++){
            dataa[i] = (dataout >> (i*8)) & 0x000000ff;
        }
        datatem = (int8_t)dataa[b%4];
        intre = (int32_t)datatem;
        finalre = (uint32_t)intre;
        err = mips_cpu_set_register(res, r2, finalre);
    }
    else err = mips_ExceptionInvalidAddress;
    
    return err;
}


mips_error lbu(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t b;
    uint32_t dataout;
    uint8_t dataa[4];
    uint32_t newb;
    uint32_t finalre;
    err = mips_cpu_get_register(res, r1, &a);
    b = a + imme;
    if((int32_t)b >= 0 && (int32_t)b < (1<<20)){
        newb = b - (b % 4);
        err = mips_mem_read(res->mem, newb, 4, (uint8_t*)&dataout);
        for(int i=0;i<4;i++){
            dataa[i] = (dataout >> (i*8)) & 0x000000ff;
        }
        finalre = (uint32_t)dataa[b%4];
        err = mips_cpu_set_register(res, r2, finalre);
    }
    else err = mips_ExceptionInvalidAddress;
    
    return err;
}

mips_error lui(unsigned r2, uint16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t result;
    result = imme << 16;
    err = mips_cpu_set_register(res, r2, result);
    return err;
}

mips_error lw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t b;
    uint32_t dataout;
    err = mips_cpu_get_register(res, r1, &a);
    b = a + imme;
    if((int32_t)b >= 0 && (int32_t)b < (1<<20)){
        if((b & 3) == 0){
            err = mips_mem_read(res->mem, b, 4, (uint8_t*)&dataout);
            dataout = byteswap(dataout);
            err = mips_cpu_set_register(res, r2, dataout);
        }
        else{
            err = mips_ExceptionInvalidAlignment;
        }
    }
    else{
        err = mips_ExceptionInvalidAddress;
    }
    
    return err;
}

mips_error lwl(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a, c;
    uint32_t result=0;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &c);
    uint32_t b;
    uint32_t dataout;
    b = a + imme;
    if((int32_t)b >= 0 && (int32_t)b < (1<<20)){
        int bdy;
        bdy = (b%4);
        b = b-(b%4);
        err = mips_mem_read(res->mem, b, 4, (uint8_t*)&dataout);
        dataout = byteswap(dataout);
        result = dataout << (bdy*8);
        switch (bdy) {
            case 3:
                result = result |(c & 0x00FFFFFF);
                break;
            case 2:
                result = result |(c & 0x0000FFFF);
                break;
            case 1:
                result = result |(c & 0x000000FF);
                break;
            case 0:
                result = result;
                break;
        }
        err = mips_cpu_set_register(res, r2, result);
    }
    else{
        err = mips_ExceptionInvalidAddress;
    }
    return err;
    
}

mips_error lwr(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a, c;
    uint32_t result=0;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &c);
    uint32_t b;
    uint32_t dataout;
    b = a + imme;
    if((int32_t)b >= 0 && (int32_t)b < (1<<20)){
        int bdy;
        bdy = (b%4);
        b = b-(b%4);
        err = mips_mem_read(res->mem, b, 4, (uint8_t*)&dataout);
        dataout = byteswap(dataout);
        result = dataout >> (bdy*8);
        switch (bdy) {
            case 0:
                result = (c&0xffffff00) | (dataout >> 24);
                break;
            case 1:
                result = (c&0xffff0000) | (dataout >> 16);
                break;
            case 2:
                result = (c&0xff000000) | (dataout >> 8);
                break;
            case 3:
                result = dataout;
                break;
            default:
                break;
        }
        err = mips_cpu_set_register(res, r2, result);
    }
    else{
        err = mips_ExceptionInvalidAddress;
    }
    return err;
    
}

mips_error mfhi(unsigned dest, mips_cpu_h res){
    mips_error err;
    err = mips_cpu_set_register(res, dest, res->HI);
    return err;
}

mips_error mflo(unsigned dest, mips_cpu_h res){
    mips_error err;
    err = mips_cpu_set_register(res, dest, res->LO);
    return err;
}

mips_error multiply(unsigned r1, unsigned r2, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    int32_t c, d;
    int64_t intresult;
    uint64_t result;
    uint32_t HI, LO;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    c = (int32_t)a;
    d = (int32_t)b;
    intresult = (c*d);
    result = (uint64_t)intresult;
    LO = (uint32_t)result;
    HI = (uint32_t)(result >> 32);
    res->HI = HI;
    res->LO = LO;
    return err;
}

mips_error multiplyunsigned(unsigned r1, unsigned r2, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint64_t result;
    uint32_t HI, LO;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = (uint64_t)((a)*(b));
    LO = (uint32_t)result;
    HI = (uint32_t)(result >> 32);
    res->HI = HI;
    res->LO = LO;
    return err;
}

mips_error orr(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = a | b;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error ori(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t finalimme;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    finalimme = (uint32_t)imme;
    result = a | finalimme;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error sb(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    uint32_t result, tmp;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = (uint32_t)(0xff & b);
    c = a + imme;
    if((int32_t)c >= 0 && (int32_t)c < (1<<20)){
        uint32_t newc = c - (c%4);
        int bdy = c%4;
        err = mips_mem_read(res->mem, newc, 4, (uint8_t*)&tmp);
        tmp = byteswap(tmp);
        switch(bdy){
            case 3:
                result = (0xffffff00 & tmp) | result;
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
                break;
            case 2:
                result = result << 8;
                result = (0xffff00ff & tmp) | result;
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
                break;
            case 1:
                result = result << 16;
                result = (0xff00ffff & tmp) | result;
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
                break;
            case 0:
                result = result << 24;
                result = (0x00ffffff & tmp) | result;
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
                break;
        }
    }
    else err = mips_ExceptionInvalidAddress;
    
    return err;
}

mips_error shw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    uint32_t result;
    uint32_t tmp;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = (uint32_t)((0x0000ffff & b));
    c = a + imme;
    if((int32_t)c >= 0 && (int32_t)c < (1<<20)){
        uint32_t newc = c - (c%4);
        err = mips_mem_read(res->mem, newc, 4, (uint8_t*)&tmp);
        tmp = byteswap(tmp);
        if((c&1) == 0){
            if((c&3) == 0){
                result = result << 16;
                result = result | (0x0000ffff & tmp);
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
            }
            else{
                result = (0xffff0000 & tmp) | result;
                result = byteswap(result);
                err = mips_mem_write(res->mem, newc, 4, (uint8_t*)&result);
            }
        
        }
        else{
            err = mips_ExceptionInvalidAlignment;
        }
    }
    else err = mips_ExceptionInvalidAddress;
    
    return err;
}

mips_error sll(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    result = a << shift;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error sllv(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = b << a;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error slt(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    int32_t c, d;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    c = (int32_t)a;
    d = (int32_t)b;
    if(c<d){
        err = mips_cpu_set_register(res, dest, 1);
    }
    else{
        err = mips_cpu_set_register(res, dest, 0);
    }
    return err;
}

mips_error slti(unsigned r1, int16_t imme, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    int32_t c;
    err = mips_cpu_get_register(res, r1, &a);
    c = (int32_t)a;
    if(c<imme){
        err = mips_cpu_set_register(res, dest, 1);
    }
    else{
        err = mips_cpu_set_register(res, dest, 0);
    }
    return err;
}

mips_error sltiu(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t finalimme;
    err = mips_cpu_get_register(res, r1, &a);
    finalimme = (uint32_t)imme;
    if(a<finalimme){
        err = mips_cpu_set_register(res, dest, 1);
    }
    else{
        err = mips_cpu_set_register(res, dest, 0);
    }
    return err;
}


mips_error sltu(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    if(a < b){
        err = mips_cpu_set_register(res, dest, 1);
    }
    else{
        err = mips_cpu_set_register(res, dest, 0);
    }
    return err;
}

mips_error sra(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    int32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    int32_t b = (int32_t)a;
    result = b >> shift;
    err = mips_cpu_set_register(res, dest, (uint32_t)result);
    return err;
}

mips_error srl(unsigned r1, unsigned dest, uint8_t shift, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    result = a >> shift;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error srlv(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    a = a & 31;
    result = b >> a;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error sub(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    int32_t c, d;
    int32_t e;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    c = (int32_t)a;
    d = (int32_t)b;
    e = (int32_t)(a - b);
    if((c > 0 && d < 0 && e < 0)||(c < 0 && d > 0 && e > 0)){
        err = mips_ExceptionArithmeticOverflow;
    }
    else{
        err = mips_cpu_set_register(res, dest, (a-b));
    }
    return err;
}

mips_error subu(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t e;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    e = a - b;
    err = mips_cpu_set_register(res, dest, e);
    return err;
}

mips_error sw(unsigned r1, unsigned r2, int16_t imme, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t c;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = b;
    result = byteswap(result);
    c = a + imme;
    if((int32_t)c >= 0 && (int32_t)c < (1<<20)){
        if((c&3) == 0){
            err = mips_mem_write(res->mem, c, 4, (uint8_t*)&result);
        }
        else{
            err = mips_ExceptionInvalidAlignment;
        }
    }
    else err = mips_ExceptionInvalidAddress;
    
    return err;
}

mips_error xorr(unsigned r1, unsigned r2, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a, b;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    err = mips_cpu_get_register(res, r2, &b);
    result = a ^ b;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

mips_error xori(unsigned r1, uint16_t imme, unsigned dest, mips_cpu_h res){
    mips_error err;
    uint32_t a;
    uint32_t finalimme;
    uint32_t result;
    err = mips_cpu_get_register(res, r1, &a);
    finalimme = (uint32_t)imme;
    result = a ^ finalimme;
    err = mips_cpu_set_register(res, dest, result);
    return err;
}

uint32_t byteswap(uint32_t memory){
    return   memory<<24| (memory & 0x0000ff00)<<8| (memory & 0x00ff0000)>>8| memory >> 24;
}