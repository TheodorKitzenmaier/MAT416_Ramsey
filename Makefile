ram: ram.o
	g++ ram.o -o ram

ram.o: graph.o
	g++ -c ram.cpp -o ram.o

graph.o:
	g++ -c graph.cpp -o graph.o

clean:
	rm -f graph.o
	rm -f ram.o
	rm -f ram
