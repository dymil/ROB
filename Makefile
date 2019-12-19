CXX=g++
CXXFLAGS=--std=c++1z

all: makeFormula test

makeFormula: makeForms.cpp

test: driver.cpp index.cpp naive.cpp naive.hpp index.hpp read_mat.hpp
	$(CXX) $(CXXFLAGS) -o $@ driver.cpp index.cpp naive.cpp

.PHONY: all clean

clean:
	rm -rf *.o *.exe *.app
