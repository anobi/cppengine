CC = g++

BUILDDIR = bin
BIN = game

SRC = src
LIBDIR = src/lib
OBJDIR = obj
OBJS = \
	$(OBJDIR)/imgui.o \
	$(OBJDIR)/imgui_draw.o \
	$(OBJDIR)/imgui_impl.o \
	$(OBJDIR)/game.o \
	$(OBJDIR)/display.o \
	$(OBJDIR)/renderer.o \
	$(OBJDIR)/input.o \
	$(OBJDIR)/controls.o \
	$(OBJDIR)/world.o \
	$(OBJDIR)/entity_transforms.o \
	$(OBJDIR)/entity_light_components.o \
	$(OBJDIR)/material_manager.o \
	$(OBJDIR)/render_entities.o \
	$(OBJDIR)/model_loader.o \
	$(OBJDIR)/shader.o \
	$(OBJDIR)/main.o

CFLAGS = -Wall -std=c++17
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

$(OBJDIR)/world.o: $(SRC)/world.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/entity_transforms.o: $(SRC)/entities/entity_transforms.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/entity_light_components.o: $(SRC)/entities/entity_light_components.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/material_manager.o: $(SRC)/rendering/material_manager.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/render_entities.o: $(SRC)/rendering/render_entities.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/model_loader.o: $(SRC)/loading/model_loader.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/shader.o: $(SRC)/shader.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OBJ)
	exec $(BUILDDIR)/$(BIN)

clean: $(OBJ)
	rm $(OBJDIR)/*.o && rm $(BUILDDIR)/$(BIN)

test: $(OBJS)
	$(CC) $(CFLAGS) -o bin/runtests \
		tests/tests_main.cpp \
		tests/test_dummy.cpp \
		tests/test_containers_array.cpp \
		tests/test_containers_queue.cpp
