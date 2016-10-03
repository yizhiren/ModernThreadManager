
CPP = g++

USER_SRCS = $(wildcard ../*/*.cpp )

USER_OBJS = $(patsubst %.cpp,%.o,$(USER_SRCS)) 

USER_BIN = demo

all: $(USER_BIN)


$(USER_BIN): $(USER_OBJS)
	$(CPP) -o $@ $(USER_OBJS)

%.o: %.cpp
	$(CPP) -c $^ -std=c++0x

clean:
	rm -rf $(USER_BIN)
	rm -rf $(USER_OBJS)

	


