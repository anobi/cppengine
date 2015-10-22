#include "Display.hpp"

#include <iostream>
#include <math.h>
#include <memory>

#include "lib/OpenGL.hpp"
#include "Renderer.hpp"

Display::Display() {
}

bool Display::Init(Renderer* renderer) {
    SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 

	_window = SDL_CreateWindow("cppengine", 32, 32, 800, 600, SDL_WINDOW_OPENGL);
	_context = SDL_GL_CreateContext(_window);
	_renderer = renderer;

	std::cout << glGetString(GL_VERSION) << " : ";

	InitGL();

    return true;
}

void Display::Update(std::vector<Entity>* entities) {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
	glRotatef(0.1f,0.2f,0.0f,0.2f);    // Rotate The cube around the Y axis

	_renderer->RenderEntities(entities);

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
    SetPerspective(width, height, 60.0f, true);
	glMatrixMode(GL_MODELVIEW);
}

void Display::SetPerspective(int width, int height, float fov, bool usePerspective) {
	GLfloat ratio = (GLfloat) width / (GLfloat) height;
    GLfloat zNear = 0.1f;
    GLfloat zFar = 255.0f;
    GLfloat fH = tan(float(fov / 360.0f * 3.14159f)) * zNear;
    GLfloat fW = fH * ratio;

    if(usePerspective) {
        glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    }
    else {
        glOrtho(-fW, fW, -fH, fH, zNear, zFar);
    }
}
