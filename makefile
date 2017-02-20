CC = clang++

BUILDDIR = bin
BIN = game

SRC = src
LIBDIR = src/lib
OBJS = $(OBJDIR)/imgui.o $(OBJDIR)/imgui_draw.o $(OBJDIR)/imgui_impl.o $(OBJDIR)/game.o $(OBJDIR)/display.o $(OBJDIR)/renderer.o $(OBJDIR)/input.o $(OBJDIR)/controls.o $(OBJDIR)/entity.o $(OBJDIR)/mesh.o $(OBJDIR)/model.o $(OBJDIR)/shader.o $(OBJDIR)/material.o $(OBJDIR)/main.o
OBJDIR = obj

CFLAGS = -Wall -std=c++11 -g
LIBS = -lSDL2 -lassimp

OS := $(shell uname)
ifeq ($(OS), Darwin)
	LIBS += -framework OpenGL -lglew
else
	LIBS += -lGL -lGLU -lGLEW
endif

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LIBS) -o bin/$@ $^

$(OBJDIR)/imgui.o: $(LIBDIR)/imgui.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/imgui_draw.o: $(LIBDIR)/imgui_draw.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/imgui_impl.o: $(LIBDIR)/imgui_impl_sdl_gl3.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/game.o: $(SRC)/game.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/display.o: $(SRC)/display.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/renderer.o: $(SRC)/renderer.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/input.o: $(SRC)/input.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/controls.o: $(SRC)/controls.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/entity.o: $(SRC)/entity.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mesh.o: $(SRC)/mesh.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/model.o: $(SRC)/model.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shader.o: $(SRC)/shader.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/material.o: $(SRC)/material.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OBJ)
	exec $(BUILDDIR)/$(BIN)

clean: $(OBJ)
	rm $(PRE) && rm $(BUILD)
