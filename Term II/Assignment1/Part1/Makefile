CXX = g++

.PHONY: all clean clean_a clean_b

all: taskA taskB

taskA: taskA.cpp
	$(CXX) -o taskA taskA.cpp

taskB: taskB.cpp
	$(CXX) -o taskB taskB.cpp

clean:
	rm -f taskA taskB
	rm -rf results/A/*
	rm -rf results/B/*

clean_a:
	rm -f taskA
	rm -rf results/A/*

clean_b:
	rm -f taskB
	rm -rf results/B/*