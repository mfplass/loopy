/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include <stdlib.h>
#include "loopy.h"

/*
 * Boot loader test
 */
int
main(int argc, char **argv)
{
    vword reg[16] = {0};
    vword mem[] = {
        16639, 768, 0, 48, 3,
        57596, 5563, 5552, 4337, 28927, 32779, 32955, 12799, 46762, 4283,
        49407, 239, 13323, 210, 53452, 218, 4350, 31, 4300, 218, 4350, 31,
        4300, 211, 60, 246, 61483, 8703, 13323, 63, 5562, 4350, 31, 173,
        4300, 61474, 111,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0
    };
    vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;
    int res;

    res = 0;
    printf("lastmem = %d (0x%x)\n", (int)lastmem, (int)lastmem);
    res += texecute(mem, lastmem, reg, 5000);
    snap(reg);
    printf("%d instructions executed\n", res);
    return(0);
}
