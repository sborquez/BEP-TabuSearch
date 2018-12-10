all: bep
	./bep -i instances/InstanceBEP-1-4-2-4.txt -o asdasd/asdasd -n 2 -l 15 -v 2

test1: bep
	./bep -i instances/InstanceBEP-1-4-2-4.txt -o asdasd/asdasd -n 2 -l 15 -v 2

test2: bep
	./bep -i instances/InstanceBEP-2-12-3-6.txt -o asdasd/asdasd -n 2 -l 15 -v 2

test3: bep
	./bep -i instances/InstanceBEP-3-11-10-7.txt -o asdasd/asdasd -n 2 -l 15 -v 2

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
