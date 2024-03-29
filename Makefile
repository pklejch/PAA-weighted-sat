CC=g++ -g -std=c++11 -Wall -pedantic

SOURCES=$(wildcard *.cpp)

all: weighted-sat

weighted-sat:
	$(CC) -o weighted-sat.out $(SOURCES) -lga

clean:
	rm weighted-sat.out
