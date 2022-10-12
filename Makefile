
SDK=1.9.6


svbtest: test.cpp
	gcc test.cpp -g -I$(HOME)/Projects/SVBCameraSDK.$(SDK)/include -Wl,-rpath=$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -pthread -lstdc++ -L$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -lSVBCameraSDK -o svbtest

#	gcc test.cpp -g -I$(HOME)/Projects/SVBCameraSDK.$(SDK)/include -Wl,-rpath=$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -pthread -lstdc++ -L$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -lSVBCameraSDK -o svbtest




clean:
	rm svbtest test.o

#	gcc test.cpp -g -I$(HOME)/Projects/SVBCameraSDK.$(SDK)/include -Wl,-rpath=$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -pthread -lstdc++ -L$(HOME)/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -lSVBCameraSDK -o svbtest


#	gcc -g -std=c++11 -I/home/stellarmate/Projects/SVBCameraSDK.$(SDK)/include -c test.cpp -o test.o
#	gcc test.o -Wl,-rpath=/home/stellarmate/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -lstdc++ -pthread -L/home/stellarmate/Projects/SVBCameraSDK.$(SDK)/lib/armv8 -lSVBCameraSDK -o svbtest

#	gcc test.c -g -L/usr/lib/aarch64-linux-gnu -lSVBCameraSDK -lpthread
