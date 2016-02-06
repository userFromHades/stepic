.PHONY: all run clean rebuild

all:
	g++ -std=c++11 -o test main.cpp

clean:
	rm test

rebuild:
	rm test
	g++ -std=c++11 -o test main.cpp

run:
	sleep 2
	./test
	ps -ela| grep test
