#!/usr/bin/env python
#  Copyright (C) 2016, Michael F. Plass
from __future__ import print_function
from lasm import A,B,C,D,E,F

def boot(a):
    '''Bootstrap loader

    Loads the initial program from a serial memory
    device that uses the SPI protocol.

    This may be anywhere in RAM, except of course
    where the program is being loaded.

    ''' 
    # Register assignments
    spw = 1;            # address of spiword routine
    rem = 2;            # remaining count of words
    start = 3;          # start address
    top = 6;            # top of loop
    #
    a.L('Boot');        # Start here
    a.addi(4, F, F);    a.C('Jump around constants');
    a.dc(0x0300);       a.C('SPI read command, high SPI address bits')
    a.dc(0x0000);       a.C('Byte address of source')
    a.dc(0x0080);       a.C('Start address of loaded program')
    a.dc(0x0800);		a.C('Program length, in words')
    a.addi(-2, F, C);	a.C('Address of program length')
    a.clr(B);           a.C('Initialize B latch')
    a.clr(0);           a.C('Clear R0')
    a.addi(1, F, spw);  a.C('R1 = address of spiword subr')
    a.jmp('L1');        a.C('Jump around subr')
    a.L('spiword')
    a.addi(-8, 0, B);   a.C('B = -8')
    a.addi(-8, B, B);   a.C('B = -16 -- bit loop counter')
    a.genop(3,1,F,F);   a.C('while B < 0')
    a.genop(-5,6,A,A);  a.C('  Shift IO through A')
    a.addi(1, B, B);    a.C('  B = B + 1')
    a.addi(-4, F, F);   a.C('  endloop')
    a.mov(E, F);        a.C('return')
    a.L('L1')
    a.genop(3,4,0,B);   a.C('  Deselect SPI')
    a.mov(D, rem);      a.C('Init word count')
    a.addi(-3, C, C);   a.C('Point at read command')
    a.mov(D, A);        a.C('Load it')
    a.addi(1, F, E);    a.C('Return address')
    a.mov(spw, F);      a.C('Call spiword')
    a.addi(1, C, C);    a.C('Second command word')
    a.mov(D, A);        a.C('Load it')
    a.addi(1, F, E);    a.C('Return address')
    a.mov(spw, F);      a.C('Call spiword')
    a.addi(1, C, C);    a.C('Point at start address')
    a.mov(D, start);    a.C('Load start address, for later')
    a.mov(start, C);    a.C('C will hold current dest addr')
    a.mov(F, top);      a.C('Top of loop')
    a.addi(-1, rem, B); a.C('Test for end')
    a.genop(2,1,F,F);   a.C('if done')
    a.genop(3,4,0,B);   a.C('  Deselect SPI')
    a.mov(start, F);    a.C('  Jump to loaded program')
    a.clr(A);           a.C('Clear A for tidiness')
    a.addi(1, F, E);    a.C('Return address')
    a.mov(spw, F);      a.C('Call spiword')
    a.mov(A, D);        a.C('Store the word we just read')
    a.addi(1, C, C);    a.C('Bump address')
    a.addi(-1,rem,rem); a.C('Decrement count')
    a.mov(top, F);      a.C('Jump to top of loop')

if __name__ == "__main__":
    import lasm
    a = lasm.assemble(boot)
    for i in range(0, len(a.inst)):
        print('%2X: %04X    %s' % (i, a.inst[i], a.comments[i]))

