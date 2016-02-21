#include "Display.hpp"

#include <iostream>
#include <math.h>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lib/OpenGL.hpp"

bool Display::Init(const int w, const int h) {

	this->width = w;
	this->height = h;
	
    SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	_window = SDL_CreateWindow("cppengine", 32, 32, this->width, this->height,
							   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	_context = SDL_GL_CreateContext(_window);

	std::cout << glGetString(GL_VERSION) << " : ";

	InitGL();

	resizing = false;
    return true;
}

void Display::Update() {

	SDL_GL_SwapWindow(_window);
}

void Display::SetResolution(const int w, const int h, bool fullScreen){
	resizing = true;
	SDL_DisplayMode displayMode;
	int getMode = SDL_GetCurrentDisplayMode(0, &displayMode);

	if(getMode != 0){
		std::cerr << "failed to get display mode" << std::endl;
	}

	this->width = w;
	this->height = h;
	
	SDL_SetWindowSize(_window, this->width, this->height);
	displayMode.w = w;
	displayMode.h = h;
	SDL_SetWindowDisplayMode(_window, &displayMode);
	SDL_GL_MakeCurrent(_window, _context);

	glViewport(0, 0, this->width, this->height);
	resizing = false;
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
    
	glViewport(0, 0, this->width, this->height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
