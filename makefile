CC = clang++
BUILD = bin/game
SRC = src/*.cpp
CFLAGS = -Wall -std=c++14
LIBS = -lSDL2main -lSDL2

OS := $(shell uname)
ifeq (OS, Darwin)
	LIBS += -framework OpenGL
else
	CFLAGS += -stdlib=libstdc++
	LIBS += -lGL
endif

all: $(OBJ)
	$(CC) $(SRC) -o $(BUILD) $(CFLAGS) $(LIBS)

run: $(OBJ)
	exec $(BUILD)
