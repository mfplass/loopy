CFLAGS=-g -Wall -Werror
PROGRAMS=test1 test2 test3 test4 test5 testboot testmultiply testdivide

default: $(PROGRAMS)
test1: test1.o execute.o util.o
test2: test2.o execute.o util.o
test3: test3.o execute.o util.o
test4: test4.o execute.o util.o
test5: test5.o execute.o util.o
testboot: testboot.o execute.o util.o
testmultiply: testmultiply.o execute.o util.o
testdivide: testdivide.o execute.o util.o
test1.o: test1.c
test2.o: test2.c
test3.o: test3.c
test4.o: test4.c
test5.o: test5.c
execute.o: execute.c
util.o: util.c
testboot.o: testboot.c
testmultiply.o: testmultiply.c
testdivide.o: testdivide.c
clean:
	rm -rf $(PROGRAMS) *.dSYM *.o
