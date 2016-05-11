CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player

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

randomtestadventurer.out: randomtestadventurer
	./randomtestadventurer > randomtestadventurer.out
	gcov dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard.out: randomtestcard
	./randomtestcard > randomtestcard.out
	gcov dominion.c >> randomtestcard.out
	cat dominion.c.gcov >> randomtestcard.out

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
			./unittest1 > unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./unittest2 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./unittest3 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./unittest4 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./cardtest1 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./cardtest2 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./cardtest3 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			echo "--------------------" >> unittestresults.out
			./cardtest4 >> unittestresults.out
			gcov dominion.c >> unittestresults.out
			cat dominion.c.gcov >> unittestresults.out

clean:
	rm -f *.o playdom.exe playdom test.exe test player randomStateGenerator randomtestadventurer randomtestcard unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.a *.dSYM
