CPP = g++
CARGS = -Wall -m64 -g -fno-inline -std=c++11

clean: 
	rm -rf grasp

grasp: src/grasp.cpp
	$(CPP) $(CARGS) -o grasp src/grasp.cpp -lpthread -lm
