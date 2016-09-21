/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include "loopy.h"

/*
 * A simple early program to try out the hardware.
 * A scope on the rB latch enable should show a 1.0000 kHz pulse.
 */

int
main(int argc, char **argv)
{
	vword reg[16] = {0};
	vword mem[] = {
	0x000a, // rA = r0 (0, assuming r0 has been cleared)
        0x000b, // rA = r0
        0x10fc, // load data address into MAR
        0x40cf, // jump around 4 data words
        0x014b, // inner loop counter (331)
        0x2222, // second data word 
	0x3333, // third data word
        0x4444, // forth data word
        0x00da, // rA = initial count
        0xf0aa, // decrement
        0x12ff, // break out of loop if rA is negative
        0xd0ff, // jump to .-3
        0x10bb, // strobe rB latch
        0x0000, // nop for timing
        0x0000, // nop for timing
        0x40cf, // end of outer infinite loop
	};
	int res = 0;
	vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;
	
	printf("lastmem = %d (0x%x)\n", (int)lastmem, (int)lastmem);
	res += texecute(mem, lastmem, reg, 20);
	snap(reg);
	res += execute(mem, lastmem, reg, 980);
	printf("----\n");
	res += texecute(mem, lastmem, reg, 20);
	snap(reg);
	res += execute(mem, lastmem, reg, 980);
	res += texecute(mem, lastmem, reg, 20);
	printf("%d instructions executed\n", res);
	snap(reg);
	return(0);
}
	
