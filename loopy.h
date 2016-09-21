/* Copyright (C) 2016, Michael F. Plass */

/**
 * vword is an unsigned integer type that determine our wordsize.
 * vsigned is the signed type of the same size.
 */
typedef unsigned short int  vword;
typedef   signed short int  vsigned;

/*
 * execute until limit instructions have been executed, or a memory
 * reference beyond lastmem is made (instruction fetch, load, or store).
 * Returns number of instructions executed.
 */
int execute(vword *mem, vword lastmem, vword reg[16], int limit);

/*
 * Same, but print as we go.
 */
int texecute(vword *mem, vword lastmem, vword reg[16], int limit);

/*
 * Print a snapshot of the registers
 */
void snap(vword reg[16]);
