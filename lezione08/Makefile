CC = g++
CFLAGS = -Wall -O3 --std=c++11

es08.exe : es08.o random.o QMparticle.o
	$(CC) random.o QMparticle.o es08.o -o es08.exe
es08.o : es08.cpp
	$(CC) -c es08.cpp -o es08.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
QMparticle.o : QMparticle.cpp QMparticle.h
	$(CC) -c QMparticle.cpp -o QMparticle.o $(CFLAGS)
clean :
	rm *.o *.exe *.out
