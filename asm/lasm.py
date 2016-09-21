#! /usr/bin/env python
#  Copyright (C) 2016, Michael F. Plass
from __future__ import print_function

A = 10
B = 11
C = 12
D = 13
E = 14
F = 15

class Assembly:
    inst = None
    cur = 0
    passno = 0
    changed = False
    symtab = None
    commenton = 0
    comments = None
    def __init__(self):
        self.inst = []
        self.symtab = dict()
        self.comments = []
    
    def v(self, sym):
        '''Get the current value of a symbol
        
        sym is the symbolic name
        '''
        ans = self.symtab.get(sym, None)
        if ans == None:
            ans = self.cur + 3
            self.changed = True
            self.symtab[sym] = ans
        return ans
    
    def equate(self, sym, val):
        '''Define the value of a symbol
        
        sym is the symbolic name
        val is the value
        '''
        if val != self.symtab.get(sym, None):
            self.symtab[sym] = (val << 0)
            self.changed = True
    
    def L(self, sym):
        '''Define a symbolic label
        
        sym is the symbolic name
        '''
        self.equate(sym, self.cur)
    
    def C(self, commentary):
        '''Comment about an instruction
        '''
        while len(self.comments) <= self.commenton:
            self.comments.append('')
        self.comments[self.commenton] = commentary
        
    def dc(self, x):
        '''Define numeric constant x
        
        >>> import lasm
        >>> a = lasm.Assembly()
        >>> a.dc(0x1234)
        >>> a.dc(0x5678)
        >>> print(a.inst)
        [4660, 22136]
        '''
        assert(x == int(x) and -32768 <= x <= 0xFFFF)
        x = x & 0xFFFF
        if self.cur == len(self.inst):
            self.inst.append(x)
            changed = True
        elif self.inst[self.cur] != x:
            self.inst[self.cur] = x
            changed = True
        self.commenton = self.cur
        self.cur = self.cur + 1;
        if (len(self.comments) > self.commenton):
            self.comments[self.commenton] = ''
    
    def addi(self, val, s, d):
        '''Add immediate instruction
        
        val is the immediate value (signed, 4 bits)
        s is the source register
        d is the destination register
        '''
        self.genop(val, 0, s, d)
    
    def mov(self, s, d):
        '''Move register instruction
        
        s is the source register
        d is the destination register
        '''
        self.genop(0, 0, s, d)
    
    def addA(self, s, d):
        '''Add A and specified register
        
        s is the addend register
        d is the destination register
        '''
        self.genop(0, 6, s, d)
    
    def subB(self, s, d):
        '''Subtract B from the specified register
        
        s is the source register
        d is the destination register
        '''
        self.genop(1, 5, s, d)
    
    def clr(self, d):
        '''Clear the destination register d
        '''
        self.genop(1, 5, B, d)
    
    def AnotB(self, d):
        '''Bitwise A & ~B operation
        
        This assumes that register 0 contains 0.
        
        d is the destination register
        '''
        self.genop(0, 7, 0, d)
    
    def orAB(self, d):
        '''Bitwise OR operation
        
        Computes (A | B)
        
        d is the destination register
        '''
        self.genop(0, 7, B, d)
        
    def jmp(self, loc):
        '''Jump
        
        Register C is altered if a short jump cannot be used
        
        loc is an absolute location, or a symbolic name
        '''
        if type(loc) == str:
            loc = self.v(loc)
        delta = loc - (self.cur + 1)
        if (-8 <= delta < 8):
            self.addi(delta, F, F)
        else:
            self.addi(1, F, C)
            self.addi(0, D, F)
            self.dc(loc)
    
    def call(self, loc):
        '''Call a subroutine
        
        Register E is used to hold the link address.
        Register C may be altered.
        
        loc is a numeric value or a symbolic name
        '''
        # Not clear the short form of this is that useful.
        # A variant that used C+1 as the return address would
        # save an instruction.
        if type(loc) == str:
            loc = self.v(loc)
        delta = loc - (self.cur + 2)
        if (-8 <= delta < 8):
            self.addi(1, F, E)
            self.addi(delta, F, F)
        else:
            self.addi(3, F, E)
            self.addi(1, F, C)
            self.addi(0, D, F)
            self.dc(loc)
    
    def genop(self, val, f, s, d):
        '''Generic loopy instruction
        
        Any loopy instruction may be specified this way,
        but for the common cases the convenience operators
        are more easily understood.
        
        val is the immediate value (signed, 4 bits)
        f contions the function bits for deriving the first addend
        s is the second addend register
        d is the destination register
        '''
        assert(val == int(val))
        if val > 7:
            val = val - 0x10000
        assert(-8 <= val < 8)
        assert(0 <= f < 16)
        assert(0 <= s < 16)
        assert(0 <= d < 16)
        self.dc((val << 12) + (f << 8) + (s << 4) + (d << 0))

def assemble(f):
    '''
    Assembler for loopy code
    
    The code to be assembled should be packaged into a function
    that takes an assembly object.  During the assembly process
    it will be called multiple times, until the values of all
    the symbols have a known value.
    
    Example:
    
    >>> import lasm
    >>> def simple(a):
    ...     from lasm import B, C, F
    ...     a.L('Start')
    ...     a.addi(1, F, C)
    ...     a.jmp('run')
    ...     a.equate('data', a.cur);
    ...     a.dc(0x1234); a.C('Data')
    ...     a.dc(0x5678)
    ...     a.dc(0xABCD)
    ...     a.L('run')
    ...     a.clr(B)
    ...     a.genop(1, 5, B, B)
    ...     a.jmp(a.cur)
    ...     a.jmp('Start')
    ... 
    >>> x = lasm.assemble(simple)
    >>> x.inst
    [4348, 12543, 4660, 22136, 43981, 5563, 5563, 61695, 4348, 223, 0]
    >>> x.comments
    ['', '', 'Data', '', '', '', '', '', '', '', '']
    '''
    a = Assembly()
    a.passno = 1
    f(a)
    a.passno = 2
    tries = 0
    while a.changed:
        assert(tries < 10)
        a.cur = 0
        a.changed = False
        f(a)
    while len(a.comments) < len(a.inst):
            a.comments.append('')
    return a
    
if __name__ == "__main__":
    import doctest
    doctest.testmod()
