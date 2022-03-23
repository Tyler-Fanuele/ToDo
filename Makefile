CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++17

all : todo

todo:main.o
	$(CC) $(CFLAGS) -o todo main.o

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

lint : main.cpp
	cpplint --filter=-runtime/references main.cpp 

clean:
	rm -f main.o
	rm -f todo
