/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include "loopy.h"

int
main(int argc, char **argv)
{
	vword reg[16] = {0};
	vword mem[] = {
		0x1000,	// r0 += 1 
		0x2000, // r0 += 2
		0xf011, // r1 += -1
		0x10fc, // load data address into MAR
		0x20cf, // jump around data
		0x1234, // data
		0x5678, // more data
		0x00db, // load first data word into rB
		0x00da, // also into rA
		0x0602, // r2 = r0 + rA
		0x0503, // r3 = r0 + (-1 - rB)
		0x1504, // r4 = r0 + (-1 - rB) + 1 
		0x10cc, // increment MAR
		0x004d, // store into second data word
		0x0E45, // save carry of rA+r4 in r5
		0x15b8, // clear r8
		0x10fc, // inline data address
		0x10cf, // jump around it
		0x0006, // data
		0x00d6, // r6 = 6 (r6 will be the counter)
		0xf86b, // rB = (r6 == 0) ? 0 : ~0
		0x31ff, // jump to .+3 if false
		0x7088, // r8 = r8 + 7
		0xf066, // r6 = r6 - 1
		0xb0ff, // loop back to .-5
		0x70ff, // jump ahead to oblivion
	};
	int res;
	vword lastmem = sizeof(mem)/sizeof(mem[0]) - 1;
	
	printf("lastmem = %d (0x%x)\n", (int)lastmem, (int)lastmem);
	res = execute(mem, lastmem, reg, 12);
	printf("----\n");
	res += texecute(mem, lastmem, reg, 100);
	printf("%d instructions executed\n", res);
	snap(reg);
	return(0);
}
	
