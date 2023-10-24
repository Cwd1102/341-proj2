CXX = g++
CXXFlags = -Wall

proj2: satnet.o mytest.cpp
	$(CXX) $(CXXFlags) satnet.o mytest.cpp -o proj2
satnet.o: satnet.cpp satnet.h
	$(CXX) $(CXXFlags) -c satnet.cpp
