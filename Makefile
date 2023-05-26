CXX = g++
CXXFLAGS = -pthread -O3 -std=c++2a -g -Wall -Wextra

all: main

clean:
	rm fork_join.o lkbucket.o read_write.o mysort 

main: fork_join.o lkbucket.o read_write.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp fork_join.o lkbucket.o read_write.o -o mysort

fork_join.o: fork_join.cpp
	$(CXX) $(CXXFLAGS) -c fork_join.cpp

lkbucket.o: lkbucket.cpp
	$(CXX) $(CXXFLAGS) -c lkbucket.cpp

read_write.o: read_write.cpp
	$(CXX) $(CXXFLAGS) -c read_write.cpp