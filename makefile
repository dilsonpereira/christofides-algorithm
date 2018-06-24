
FLAG = -O3 -m64 -Wall

Matching.o: ./Matching/Matching.cpp ./Matching/Matching.h ./Matching/BinaryHeap.h ./Matching/Globals.h ./Matching/Graph.h 
	g++ $(FLAG) -c ./Matching/Matching.cpp -o Matching.o

BinaryHeap.o: ./Matching/BinaryHeap.h ./Matching/BinaryHeap.cpp ./Matching/Globals.h
	g++ $(FLAG) -c ./Matching/BinaryHeap.cpp -o BinaryHeap.o

Graph.o: ./Matching/Graph.h ./Matching/Graph.cpp
	g++ $(FLAG) -c ./Matching/Graph.cpp -o Graph.o

TSPLIB_parser.o: TSPLIB_parser.cpp TSPLIB_parser.h ./Matching/Graph.h 
	g++ $(FLAG) -c TSPLIB_parser.cpp -o TSPLIB_parser.o

Example.o: Example.cpp MST.h ./Matching/Matching.h ./Matching/Graph.h Christofides.h TSPLIB_parser.h
	g++ $(FLAG) -c Example.cpp -o Example.o

christofides: Matching.o BinaryHeap.o Graph.o Example.o TSPLIB_parser.o
	g++ $(FLAG) Matching.o BinaryHeap.o Graph.o Example.o TSPLIB_parser.o -o christofides

