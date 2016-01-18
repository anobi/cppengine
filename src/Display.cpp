#include "Display.hpp"

#include <iostream>
#include <math.h>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lib/OpenGL.hpp"

Display::Display() {
}

bool Display::Init() {
    SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 

	_window = SDL_CreateWindow("cppengine", 32, 32, 800, 600, SDL_WINDOW_OPENGL);
	_context = SDL_GL_CreateContext(_window);

	std::cout << glGetString(GL_VERSION) << " : ";

	InitGL();

    return true;
}

void Display::Update() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)800 / (float)600, 0.1f, 1000.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0));// Head is up (set to 0,-1,0 to look upside-down)
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 mvp = Projection * View * Model;

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
    
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Display::SetViewport(int width, int height) {

	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}
