CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

testdominion: testdominion.c dominion.o
	gcc -o testdominion testdominion.c -g dominion.o rngs.o $(CFLAGS)

testdominion.out: testdominion
	./testdominion > testdominion.out
	gcov dominion.c >> testdominion.out
	cat dominion.c.gcov >> testdominion.out

clean:
	rm -f *.o testdominion *.gcov *.gcda *.gcno *.so *.a *.dSYM