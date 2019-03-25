CC = clang
CXX = clang++
CXXFLAGS ?= -std=c++17 -O2 -g

decode:
	$(CXX) $(CXXFLAGS) -o decode hw01/main.cpp
	
cfg:
	$(CXX) $(CXXFLAGS) -o cfg hw02/main.cpp