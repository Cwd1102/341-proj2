CXX = g++
CXXFlags = -Wall

proj2: satnet.o driver.cpp
	$(CXX) $(CXXFlags) csr.o driver.cpp -o proj2
satnet.o: satnet.cpp satnet.h
	$(CXX) $(CXXFlags) -c satnet.cpp
