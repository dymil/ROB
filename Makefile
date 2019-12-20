CXX=g++
CXXFLAGS=--std=c++1z

all: formula test

formula: makeForms.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: driver.cpp index.cpp naive.cpp naive.hpp index.hpp read_mat.hpp
	$(CXX) $(CXXFLAGS) -o $@ driver.cpp index.cpp naive.cpp

readMat: read_mat.cpp read_mat.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: all clean

clean:
	rm -rf *.o *.exe *.app
