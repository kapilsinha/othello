CC          = g++
CFLAGS      = -std=c++11 -Wall -pedantic -ggdb
OBJS        = node.o player.o
PLAYERNAME  = DecentPlayer

all: $(PLAYERNAME) testgame

$(PLAYERNAME): $(OBJS) wrapper.o
	$(CC) -o $@ $^
	
testnode: $(OBJS) testnode.o
	$(CC) -o $@ $^

testgame: testgame.o
	$(CC) -o $@ $^

testminimax: $(OBJS) testminimax.o
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) -c $(CFLAGS) -x c++ $< -o $@

java:
	make -C java/

cleanjava:
	make -C java/ clean

clean:
	rm -f *.o $(PLAYERNAME) testgame testminimax

.PHONY: java testminimax
