all: bep
	./bep -i instances/InstanceBEP-1-4-2-4.txt -o asdasd/asdasd -n 10 -l 15 -v 2

bep: main.o tabusearch.o representation.o
	g++ -std=c++11 -Wall -o bep main.o tabusearch.o representation.o

main.o: main.cpp tabusearch.hpp
	g++ -std=c++11 -Wall -c main.cpp

tabusearch.o: tabusearch.cpp tabusearch.hpp representation.hpp
	g++ -std=c++11 -Wall -c tabusearch.cpp

representation.o: representation.cpp representation.hpp
	g++ -std=c++11 -Wall -c representation.cpp

clean:
	rm -f *.o bep
