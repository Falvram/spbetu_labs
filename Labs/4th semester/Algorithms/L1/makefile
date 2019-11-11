all: main.o matrix.o
	g++ main.o matrix.o -o out
main.o: main.cpp matrix.cpp matrix.h 
	g++ -c main.cpp
matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp