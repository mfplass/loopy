/* Copyright (C) 2016, Michael F. Plass */
#include <stdio.h>
#include "loopy.h"

void
snap(vword reg[16])
{
	int i;
	for (i = 0; i < 16; i++)
		printf("%X: %04x%s",
			   i, (unsigned)reg[i], ((i%8)==7)?"\n":"  ");
}

int
texecute(vword *mem, vword lastmem, vword reg[16], int limit)
{
	int i;
	int res = 0;
	vword pc;
	unsigned inst;
	unsigned val;
	int d;
	
	for (i = 0, pc=reg[0xF]; i < limit && pc <= lastmem; i++) {
		inst = mem[pc];
		res = execute(mem, lastmem, reg, 1);
		if (res == 0) {
			printf("%5d: %04x\n", (int)pc, inst);
			return(i);
		}
		d = inst & 0xf;
		val = reg[d];
		if (d == 0xD && reg[0xC] <= lastmem)
			val = mem[reg[0xC]];
		printf("%5d: %04x %04x\n", (int)pc, inst, val);
		pc = reg[0xF];
	}
	return(i);
}
