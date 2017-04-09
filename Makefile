CPP = g++
CARGS = -Wall -m64 -fno-inline -std=c++11

clean: 
	rm -rf graspDebug grasp reactivegraspDebug reactivegrasp

graspDebug: src/grasp.cpp
	$(CPP) $(CARGS) -g -o graspDebug src/grasp.cpp -lpthread -lm

grasp: src/grasp.cpp
	$(CPP) $(CARGS) -o3 -o grasp src/grasp.cpp -lpthread -lm

reactivegraspDebug: src/reactivegrasp.cpp
	$(CPP) $(CARGS) -g -o reactivegraspDebug src/reactivegrasp.cpp -lpthread -lm

reactivegrasp: src/reactivegrasp.cpp
	$(CPP) $(CARGS) -o3 -o reactivegrasp src/reactivegrasp.cpp -lpthread -lm
