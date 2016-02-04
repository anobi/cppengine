CC = g++
BUILD = bin/game
SRC = src/*.cpp
CFLAGS = -Wall -std=c++14 -g
LIBS = -lSDL2 -lassimp

OS := $(shell uname)
ifeq ($(OS), Darwin)
	LIBS += -framework OpenGL -lglew
else
	LIBS += -lGL -lGLU -lGLEW
endif

all: $(OBJ)
	$(CC) $(SRC) -o $(BUILD) $(CFLAGS) $(LIBS)

run: $(OBJ)
	exec $(BUILD)

clean: $(OBJ)
	rm $(PRE) && rm $(BUILD)
