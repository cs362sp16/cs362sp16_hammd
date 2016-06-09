CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

unittest1: unittest1.c dominion.o
	gcc -o unittest1 unittest1.c -g dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o
	gcc -o unittest2 unittest2.c -g dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o
	gcc -o unittest3 unittest3.c -g dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o
	gcc -o unittest4 unittest4.c -g dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o
	gcc -o cardtest1 cardtest1.c -g dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o
	gcc -o cardtest2 cardtest2.c -g dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o
	gcc -o cardtest3 cardtest3.c -g dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o
	gcc -o cardtest4 cardtest4.c -g dominion.o rngs.o $(CFLAGS)

randomStateGenerator: randomStateGenerator.c dominion.o
	gcc -o randomStateGenerator randomStateGenerator.c -g dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c randomStateGenerator.o dominion.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o randomStateGenerator.o $(CFLAGS)

randomtestcard: randomtestcard.c randomStateGenerator.o dominion.o
	gcc -o randomtestcard randomtestcard.c -g dominion.o rngs.o randomStateGenerator.o $(CFLAGS)

testdominion: testdominion.c dominion.o
	gcc -o testdominion testdominion.c -g dominion.o rngs.o $(CFLAGS)

randomtestadventurer.out: randomtestadventurer
	./randomtestadventurer > randomtestadventurer.out
	gcov dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

cardtest1.out: cardtest1
	./cardtest1 > cardtest1.out
	gcov dominion.c >> cardtest1.out
	echo "--------------------" >> cardtest1.out

test.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestcard randomtestadventurer testdominion
	./unittest1 > test.out
	echo "--------------------" >> test.out
	./unittest2 >> test.out
	echo "--------------------" >> test.out
	./unittest3 >> test.out
	echo "--------------------" >> test.out
	./unittest4 >> test.out
	echo "--------------------" >> test.out
	./cardtest1 >> test.out
	echo "--------------------" >> test.out
	./cardtest2 >> test.out
	echo "--------------------" >> test.out
	./cardtest3 >> test.out
	echo "--------------------" >> test.out
	./cardtest4 >> test.out
	echo "--------------------" >> test.out
	./randomtestcard >> test.out
	echo "--------------------" >> test.out
	./randomtestadventurer >> test.out
	echo "--------------------" >> test.out
	./testdominion >> test.out
	echo "--------------------" >> test.out
	gcov dominion.c >> test.out
	cat dominion.c.gcov >> test.out

clean:
	rm -f *.o test testdominion randomStateGenerator randomtestadventurer randomtestcard unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 *.gcov *.gcda *.gcno *.so *.a *.dSYM
