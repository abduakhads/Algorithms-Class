CXX = g++

task: task.cpp
	$(CXX) -o task task.cpp

.PHONY: clean clean_py

clean:
	rm -f task

clean_py:
	rm -rf __pycache__
	rm -rf .pytest_cache
