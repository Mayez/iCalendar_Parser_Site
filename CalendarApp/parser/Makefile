all: compile

CalendarParser.o: src/CalendarParser.c
	gcc -Wall -g -Iinclude -std=c11 -fpic -c src/CalendarParser.c -o CalendarParser.o 

LinkedListAPI.o: src/LinkedListAPI.c
	gcc -Wall -g -Iinclude -std=c11 -fpic -c src/LinkedListAPI.c -o LinkedListAPI.o 

compile: LinkedListAPI.o CalendarParser.o
	gcc -shared -o ../libcal.so LinkedListAPI.o CalendarParser.o

clean:
	rm -f *.o *.a *.so ;
	rm ../libcal.so
