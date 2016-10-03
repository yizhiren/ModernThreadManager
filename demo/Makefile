
CPP = g++

USER_SRCS = $(wildcard ./*.cpp ./*/*.cpp )

USER_OBJS = $(patsubst %.cpp,%.o,$(src)) 

USER_BIN = demo

all: $(USER_BIN)


$(USER_BIN): $(USER_OBJS)
	$(CPP) -o $@ $(USER_OBJS)

%.cpp: %.o
	$(CPP) -c $@


	


