all: bep
	./bep -i asdas -o asdasd/asdasd -n 10 -l 15 -v 2

bep: main.o tabusearch.o representation.o
	g++ -Wall -o bep main.o tabusearch.o

main.o: main.cpp tabusearch.hpp
	g++ -Wall -c main.cpp

tabusearch.o: tabusearch.cpp tabusearch.hpp representation.hpp
	g++ -Wall -c tabusearch.cpp

representation.o: representation.cpp representation.hpp
	g++ -Wall -c representation.cpp

clean:
	rm -f *.o bep
