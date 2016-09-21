/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include <stdlib.h>
#include "loopy.h"

/*
 * Multiply
 */
int
main(int argc, char **argv)
{
    vword reg[16] = {0};
    vword mem[] = {
        0x50FF, // Skip 5 data words
        0x0038, // 56
        0xf4d1, // 62673 - dividend is 3732689
        0x0321, // 801 - divisor
        0,      // space for quotient
        0,      // space for remainder
        0x15B0, // r0 = 0
        0x100c, // C = 1
        0x00d1, // r1 = 56
        0x10cc, // C = C + 1
        0x00d2, // r2 = 62673
        0x10cc, // C = C + 1
        0x00d3, // r3 = 801
        0x10FE, // return address
        0x50FF, // call divide
        0x400C, // C = 4
        0x00AD, // mem[4] = quotient
        0x10CC, // C = 5
        0x001D, // mem[5] = remainder
        0x800F, // halt
        // Divide routine
        // r1 - high half of dividend - must be smaller than divisor
        // r2 - low half of dividend
        // r3 - divisor (max 32768)
        // rE - return address
        // Quotient is returned in A, reminder in r1
        // Variables:
        // B is divisor
        // r3 temp
        // r4 accumulates quotient
        // r5 is loop counter
        // r6 is loop top
        0x15B4, // r4 = 0
        0x7045, // r5 = 7
        0x7055, // r5 = 14
        0x1055, // r5 = 15
        0x003B, // B = divisor
        0x00F6, // r6 = top of loop
        0x004A, // A = r4
        0x0644, // r4 = r4 + A
        0x002A, // A = r2
        0x1213, // r3 = r1 + HIBIT(A)
        0x0622, // r2 = r2 + A
        0x001A, // A = r1
        0x0631, // r1 = r3 + A
        0x1D1A, // r1 : B
        0x12FF, // blt .+1
        0x20FF, // b .+2
        0x1044, // Accumulate quotient in r4
        0x1511, // r1 = r1 - B
        0xF055, // r5 = r5 - 1
        0x005A, // Test r5
        0x12FF, // Skip if negative
        0x006F, // go to top of loop
        0x004A, // return quotent in A
        0x00EF, // return
    };
    vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;
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
    printf("dividend = %d(%#x), divisor = %d(%#x)\n",
           (int)mem[1] * 65536 + mem[2],
           (int)mem[1] * 65536 + mem[2],
           (int)mem[3], (int)mem[3]);
    res += texecute(mem, lastmem, reg, 12);
    snap(reg);
    res += execute(mem, lastmem, reg, 400);
    snap(reg);
    printf("%d instructions executed\n", res);
    printf("quotient = %d(%#x), remainder = %d(%#x)\n",
           (int)mem[4], (int)mem[4], (int)mem[5], (int)mem[5]);
    return(0);
}
