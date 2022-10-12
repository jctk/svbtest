
TARGET := SVBDemo
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

$(TARGET) : $(OBJS)
	gcc $< -Wl,-rpath=../lib/x64 -pthread -lstdc++ -L../lib/x64 -lSVBCameraSDK -o $@

%.o:%.cpp
	gcc -g -std=c++11 -I../include -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

distclean:
	rm -rf $(OBJS) $(TARGET) *.bin *.raw

.PHONY: clean distclean


