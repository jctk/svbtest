
SDK=1.9.6

svbtest: svbtest.cpp
	gcc svbtest.cpp -g -I./SVBCameraSDK.$(SDK)/include -Wl,-rpath=./SVBCameraSDK.$(SDK)/lib/armv8 -pthread -lstdc++ -L./SVBCameraSDK.$(SDK)/lib/armv8 -lSVBCameraSDK -o svbtest

clean:
	rm svbtest
