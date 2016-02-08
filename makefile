.PHONY: run clean rebuild

all:
	g++ -std=c++11 -DBATLE -o test main622.cpp -lrt
