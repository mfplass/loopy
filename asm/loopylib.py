#!/usr/bin/env python
#  Copyright (C) 2016, Michael F. Plass
from lasm import A,B,C,D,E,F

def multiply(a):
    '''Multiply routine

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
    
    >>> import lasm
    >>> x = lasm.assemble(multiply)
    >>> x.inst
    [5555, 4100, 61445, 246, 63530, 8959, 58, 239, 75, 42, 1794, 1291, 3931, 26, 4607, 1587, 1697, 74, 1700, 111]
    '''
    a.L("Multiply")
    a.clr(3)            # r3 = 0
    a.addi(1, 0, 4)     # r4 = 1 (bit mask)
    a.addi(-1, 0, 5)    # r5 = -1
    a.mov(F, 6)         # Top of loop
    a.genop(-1, 8, 2, A)# hibit(A) = (r2 != 0)
    a.genop(2, 2, F, F) # if (r2 == 0)
    a.mov(3, A)         #   A = r3
    a.mov(E, F)         #   return
    a.mov(4, B)         # mask bit to B
    a.mov(2, A)         # A = multiplier
    a.AnotB(2)          # update multiplier: r2 = A & ~B
    a.genop(0, 5, 0, B) # B = ~B
    a.genop(0, F, 5, B) # hibit(B) = (current multiplier bit != 0)
    a.mov(1,A)          # A = multiplicand
    a.genop(1, 1, F, F) # if hibit(B)
    a.addA(3, 3)        #   r3 = r3 + A
    a.addA(A, 1)        # r1 = A + A
    a.mov(4, A)         # A = maskbit
    a.addA(A, 4)        # maskbit = A + A
    a.mov(6, F)         # go to top of loop

if __name__ == "__main__":
    import doctest
    doctest.testmod()
