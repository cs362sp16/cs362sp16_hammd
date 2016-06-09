CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

testdominion: testdominion.c dominion.o
	gcc -o testdominion testdominion.c -g dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o
	gcc -o cardtest1 cardtest1.c -g dominion.o rngs.o $(CFLAGS)

testdominion.out: testdominion
	./testdominion > testdominion.out
	gcov dominion.c >> testdominion.out
	cat dominion.c.gcov >> testdominion.out

randomtestadventurer: randomtestadventurer.c randomStateGenerator.o dominion.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o randomStateGenerator.o $(CFLAGS)

randomtestadventurer.out: randomtestadventurer
	./randomtestadventurer > randomtestadventurer.out
	gcov dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

cardtest1.out: cardtest1
	./cardtest1 > cardtest1.out
	gcov dominion.c >> cardtest1.out
	echo "--------------------" >> cardtest1.out
	
clean:
	rm -f *.o testdominion *.gcov *.gcda *.gcno *.so *.a *.dSYM