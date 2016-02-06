.PHONY: all run clean rebuild

all:
	g++ -std=c++11 -o test main_1.cpp

clean:
	rm test

run:
	sleep 2
	./test
	ps -ela| grep test
