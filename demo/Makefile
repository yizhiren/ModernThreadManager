CXX ?= g++

USER_SRCS = $(wildcard ../*/*.cpp )

USER_OBJS = $(patsubst %.cpp,%.o,$(USER_SRCS)) 

USER_BIN = demo

COMPILE_FLAG = -std=c++11 -lpthread

LINK_FLAG = -lpthread

all: $(USER_BIN)


$(USER_BIN): $(USER_OBJS)
	$(CXX) -o $@ $(LINK_FLAG) $(USER_OBJS)

%.o: %.cpp
	$(CXX) -c $^ $(COMPILE_FLAG) -o $@

clean:
	rm -rf $(USER_BIN)
	rm -rf $(USER_OBJS)

	


