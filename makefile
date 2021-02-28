CC = g++

BUILDDIR = bin
BIN = game

SRC = src
LIBDIR = src/lib
OBJDIR = obj
OBJS = $(OBJDIR)/imgui.o $(OBJDIR)/imgui_draw.o $(OBJDIR)/imgui_impl.o $(OBJDIR)/game.o $(OBJDIR)/display.o $(OBJDIR)/renderer.o $(OBJDIR)/input.o $(OBJDIR)/controls.o $(OBJDIR)/entity.o $(OBJDIR)/render_mesh.o $(OBJDIR)/render_material.o $(OBJDIR)/model_loader.o $(OBJDIR)/model.o $(OBJDIR)/shader.o $(OBJDIR)/main.o

CFLAGS = -Wall -std=c++14
LIBS = -lSDL2 -lassimp

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O3
endif

OS := $(shell uname)
ifeq ($(OS), Darwin)
	CC = clang++
	LIBS += -framework OpenGL -lglew -I/usr/local/include -L/usr/local/lib -lSDL2
else
	LIBS += -lGL -lGLU -lGLEW
endif

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LIBS) -o bin/$@ $^
	cp shaders/* $(BUILDDIR)/shaders/

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

$(OBJDIR)/render_material.o: $(SRC)/rendering/render_material.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/render_mesh.o: $(SRC)/rendering/render_mesh.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/model_loader.o: $(SRC)/loader/model_loader.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/model.o: $(SRC)/model.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shader.o: $(SRC)/shader.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OBJ)
	exec $(BUILDDIR)/$(BIN)

clean: $(OBJ)
	rm $(OBJDIR)/*.o && rm $(BUILDDIR)/$(BIN)

test: $(OBJS)
	$(CC) -o bin/runtests tests/tests_main.cpp tests/test_dummy.cpp