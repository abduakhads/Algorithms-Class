CXX = g++

.PHONY: all clean

all: taskA taskB

taskA: taskA.cpp
	$(CXX) -o taskA taskA.cpp

taskB: taskB.cpp
	$(CXX) -o taskB taskB.cpp

clean:
	rm -f taskA taskB
