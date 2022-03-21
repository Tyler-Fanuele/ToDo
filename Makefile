CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++17

todo:main.o
	$(CC) $(CFLAGS) -o todo main.o
main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm -f main.o
	rm -f todo
