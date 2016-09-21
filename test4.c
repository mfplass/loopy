/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include <stdlib.h>
#include "loopy.h"

/*
 * wig-wag runs back and forth
 */
int
main(int argc, char **argv)
{
    vword reg[16] = {0};
    vword mem[] = {
        0x15B0, // r0 = 0
        0x10FC, // C = data area
        0x00DF, // jump to entry
        0x000d, // L1
        0x800B, // ROTR1: B = -8
        0x90BB, // B = -15
        0x12A0, // r0 = A + HIGHBIT(A)
        0x060A, // A = A + r0
        0x10BB, // B = B + 1
        0x11FF, // break if B >= 0
        0xB0FF, // loop
        0x15B0, // r0 = 0
        0x00EF, // return
        0x0001, // L1: r1 = false -- keeps track of direction
        0x1002, // r2 = 1 -- runner
        0x00F6, // E = top of loop
        0x001B, // test direction
        0x31FF, // if moving left, then
        0x10FC,
        0x00DF,
        0x001d, // L2
        0x002A, //   A = runner
        0x22FF, //   if room to move left,
        0x06AA, //    A = A + A
        0x10FF, //   else
        0xF001, //    change to moving right
        0x10FC, //
        0x00DF, // 
        0x0029, // else (L3)
        0x002B, //  L2: B = runner
        0x100A, //  A = 1
        0x070A, //  A = (A & ~B)
        0xF8AA, //  test A == 0 (i.e., LOWBIT(runner)==1)
        0x42FF, //  if so,
        0x0001, //   change to moving left
        0x2022, //   mutate pattern
        0x002A, //   A = runner
        0x30FF, //  else
        0x002A, //   A = runner
        0x10FE, //   link
        0x400F, //   call ROTR1
        0x00A2, // L3: runner = A
        0x002A, // show runner
        0x0000, // NOP
        0x0000, // NOP
        0x0000, // NOP
        0x006F, // loop
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
    res += texecute(mem, lastmem, reg, 4);
    snap(reg);
    res += texecute(mem, lastmem, reg, 5000);
    snap(reg);
    printf("%d instructions executed\n", res);
    return(0);
}
