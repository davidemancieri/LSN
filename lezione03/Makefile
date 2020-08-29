CC = g++
CFLAGS = -Wall -O3 --std=c++11

es03_1.exe : es03_1.o random.o pricing.o
	$(CC) random.o pricing.o es03_1.o -o es03_1.exe
es03_1.o : es03_1.cpp
	$(CC) -c es03_1.cpp -o es03_1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
pricing.o : pricing.cpp pricing.h
	$(CC) -c pricing.cpp -o pricing.o $(CFLAGS)
clean :
	rm *.o *.exe *.out
