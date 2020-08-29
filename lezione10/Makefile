CC = g++
CFLAGS = -Wall -O3 --std=c++11

es10.exe : es10.o random.o class.o
	$(CC) random.o class.o es10.o -o es10.exe
es10.o : es10.cpp
	$(CC) -c es10.cpp -o es10.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
class.o : class.cpp class.h
	$(CC) -c class.cpp -o class.o $(CFLAGS)
clean :
	rm *.o *.exe *.dat
