/* Copyright (C) 2016, Michael F. Plass */
#include "loopy.h"

#define IMBT 12		/* low-order bit of immediate value */
#define SHRT 11   	/* result (with carry)  is shifted right by 1 */
#define ISBT 10  	/* immediate select bit */
#define ASBT 9   	/* A select */
#define BSBT 8   	/* B complement select */

int
execute(vword mem[], vword lastmem, vword reg[16], int limit)
{
	vword pc;	/* program counter */
	int d, s;	/* register ids (really 4 bits) */
	unsigned t;	/* must be larger that vword */
	vword x, y, imm, cy;
	vsigned ir;	/* instruction being executed */
	int i;		/* counts instructions executed */

	pc = reg[0xF];
	for (i = 0; i < limit && pc <= lastmem; i++) {
		ir = mem[pc];
		d = ir & 0xF;		/* destination register */
		s = (ir >> 4) & 0xF;	/* source register */
		if ((s == 0xD || d == 0xD) && reg[0xC] > lastmem)
			break;
		pc = pc + 1;		/* increment program counter */
		imm = (ir >> IMBT); /* sign-extend immediate value */
		x = ~0; /* initial value */
		if ((ir >> ASBT) & 1)
			x &= reg[0xA];	/* conditional AND with A */
		if ((ir >> BSBT) & 1)
			x &= ~reg[0xB]; /* conditional AND with NOT B */
		if ((ir >> ISBT) & 1)
			cy = imm & 1;   /* use only low-order bit of imm in this case */
		else {
			x = (((vsigned)x) < 0) ? imm : 0;
			cy = 0;
		}
		y = (s == 0xF) ? pc : (s == 0xD) ? mem[reg[0xC]] : reg[s];
		t = x + y + cy;		/* adder at work */
		if ((ir >> SHRT) & 1)	/* result is shifted right by 1 bit */
			t = t >> 1;
		if (d == 0xF)
			pc = t;		/* store to pc */
		else if (d == 0xD)
			mem[reg[0xC]] = t; /* store to memory */
		else
			reg[d] = t;	/* vanilla store to register */
	}
	reg[0xF] = pc;
	return(i);
}
