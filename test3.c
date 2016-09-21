/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include <stdlib.h>
#include "loopy.h"

/*
 * Feedback shift register based on the irreducable polynomial over GF(2)
 * Order:= 65535; Irreduc(X^16 + X^6 + X^4 + X + 1) mod 2
 *
 * First 21 terms:
 * 1, 3, 7, 15, 30, 61, 123, 247, 494, 988, 1977, 3955, 7910, 15821, 31642,
 * 63285, 61035, 56534, 47533, 29530, 59061
 */
/******
 def nxt(p = [1]):
     x = p[0]
     x = x + x + (((x >> 15) + (x >> 5) + (x >> 3) + x) & 1)
     x = x & 0xffff
     p[0] = x
     return x
 
 p = [nxt() for i in range(0,65535)]
 p[:20]
******/
int
main(int argc, char **argv)
{
    vword reg[16] = {0};
    vword mem[] = {
        0x40FF, // Skip 4 data words
        0x8000, // 
        0x0020, // 
        0x0008, //
        0x0001, //
        0x15B0, // r0 = 0
        0x100A, // A = 1
        0x00FE, // E = top of loop
        0x100C, // C = 1 -- picks up terms
        0x00AB, // B = A
        0x050B, // B = ~B
        0x06A2, // r2 = A + A
        0x0004, // r4 = 0
        0x00DA, //  A = mask
        0x0703, //  r3 = A & ~B
        0xF83A, //  A = (r3 != 0)
        0x0644, //  r4 = r4 + A
        0x10CC, //  C = C + 1
        0xB0CA, //  A = C - 4
        0x92FF, //  If A < 0 goto .-7
        0x004A, // A = r4
        0xE00B, // B = ~1
        0x072A, // A = r2 + (A & ~B)
        0x0000, // NOP
        0x0000, // NOP
        0x0000, // NOP
        0x00EF, // loop
    };
    vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;
    int i;
    int res;

    if (argv[1]) {
        res = atoi(argv[1]);
        mem[1] = res >> 16;
        mem[2] = res & 0xffff;
        if (argv[2])
            mem[3] = atoi(argv[2]);
    }
    res = 0;
    printf("lastmem = %d (0x%x)\n", (int)lastmem, (int)lastmem);
    res += texecute(mem, lastmem, reg, 4);
    snap(reg);
    printf("A = %u\n", (unsigned)reg[0xA]);
    res += texecute(mem, lastmem, reg, 4 * 7 + 12);
    snap(reg);
    printf("A = %u\n", (unsigned)reg[0xA]);
    for (i = 0; i < 20; i++) {
        res += execute(mem, lastmem, reg, 4 * 7 + 12);
        snap(reg);
        printf("A = %u\n", (unsigned)reg[0xA]);
    }
    printf("%d instructions executed\n", res);
    return(0);
}
