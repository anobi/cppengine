#include "Display.hpp"

#include <iostream>
#include <math.h>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lib/OpenGL.hpp"

Display::Display() {
}

bool Display::Init() {
    SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	_window = SDL_CreateWindow("cppengine", 32, 32, 800, 600, SDL_WINDOW_OPENGL);
	_context = SDL_GL_CreateContext(_window);

	std::cout << glGetString(GL_VERSION) << " : ";

	InitGL();

    return true;
}

void Display::Update() {

	SDL_GL_SwapWindow(_window);
}

void Display::Shutdown() {
	std::cout << "* Display\n";
	SDL_DestroyWindow(_window);

    _context = NULL;
	_window = NULL;
}

void Display::InitGL() {

    glewExperimental = GL_TRUE;
	glewInit();
    
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
