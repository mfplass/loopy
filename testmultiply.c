/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include "loopy.h"

/*
 * Multiply
 */
int
main(int argc, char **argv)
{
	vword reg[16] = {0};
	vword mem[] = {
		0x15B0, // r0 = 0
		0x7001, // r1 = 7
		0x5002, // r2 = 5
		0x10FE, // return address
		0x600F, // call multiply
		0x800F, // halt
		// Multiply routine
		// r0 - zero (preserved)
		// r1 - multiplicand
		// r2 - multiplier
		// rE - return address
		// Result is placed in A
		// Variables:
		// r3 is used to accumulate answer
		// r4 is bit mask for the active bit
		// r5 is -1
		// r6 is loop top
		// multiplicand is shifted left at each stage
		0x0003, // r3 = 0
		0x1004, // r4 = 1 (bit mask)
		0xF005, // r5 = -1
		0x00F6, // r6 = top of loop
		0xF82A, // A = (r2 != 0)
		0x22FF, // if not A
		0x003A, //	A = r3
		0x00EF, //	return
		0x004B, // mask bit to B
		0x002A, // multiplier to A
		0x0702, // update multiplier: r2 = A & ~B
		0x050B, // B = ~B
		0x0F5B, // B = (current multiplier bit != 0)
		0x001A, // A = multiplicand
		0x11FF,	// if B
		0x0633, //     r3 = r3 + A
		0x06A1, // r1 = A + A
		0x004A, // A = maskbit
		0x06A4, // maskbit = A + A
		0x006F,	// go to top of loop
	};
	int res = 0;
	vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;

	printf("lastmem = %d (0x%x)\n", (int)lastmem, (int)lastmem);
	res += execute(mem, lastmem, reg, 1000);
	snap(reg);
	printf("%d instructions executed\n", res);
	return(0);
}
