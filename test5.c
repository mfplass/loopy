/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include <stdlib.h>
#include "loopy.h"

/*
 * wig-wag runs back and forth
 *
 * This version uses the *new* shift-right feature
 */
int
main(int argc, char **argv)
{
    vword reg[16] = {0};
    vword mem[] = {
        0x00BB, // init B latches
        0x15B0, // r0 = 0
        0x10FC, // C = data area
        0x20FF, // jump to entry
        0xFFF0, // complemented mask for pattern
        0x0002, // delay loop count - adjust on real hardware to suit
        0x00D4, // load mask into r4
        0x10CC, // next loc
        0x00D3, // r3 = delay loop value
        0x0001, // r1 = false -- keeps track of direction
        0x1002, // r2 = 1 -- runner
        0x00F6, // E = top of loop
        0x001B, // test direction
        0x31FF, // if moving left, then
        0x10FC,
        0x00DF,
        0x0019, // L2                    >>--- 
        0x002A, //   A = runner
        0x22FF, //   if room to move left,
        0x06A2, //    runner = A + A
        0x10FF, //   else
        0xF001, //    change to moving right
        0x10FC, //
        0x00DF, // 
        0x0024, // else (L3)                    >>---
        0x002B, //  L2: B = runner        <---
        0x100A, //  A = 1
        0x070A, //  A = (A & ~B)
        0xF8AA, //  test A == 0 (i.e., LOWBIT(runner)==1)
        0x52FF, //  if so,
        0x0001, //   change to moving left
        0x202A, //   mutate pattern
        0x004B, //   mask to B
        0x0702, //   apply mask
        0x30FF, //  else
        0x0822, //   runner >>= 1               
        0x002A, // L3: show runner               <---
        0x0000, // NOP
        0x003B, // loop count
        0xF0BB, // repeat B--
        0xE1FF, // until B < 0
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
