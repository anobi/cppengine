#include <iostream>
#include <math.h>
#include <memory>

//GL headers for osx/linux
#include "lib/OpenGL.hpp"

#include "Display.hpp"
#include "Renderer.hpp"

SDL_GLContext D_Context; 
GLuint vao;
int pointcount;

Display::Display(){
    D_Window = 0;
}

bool Display::Init(){
    SDL_Init(SDL_INIT_VIDEO);

	D_Window = SDL_CreateWindow("asd", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
	D_Context = SDL_GL_CreateContext(D_Window);

	InitGL();

    return true;
}

void Display::Update(){

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	//glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
	//glRotatef(0.1f,0.2f,0.0f,0.2f);    // Rotate The cube around the Y axis

    //render entities here
	
    glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pointcount);

	SDL_GL_SwapWindow(D_Window);
}

void Display::LoadMeshes(std::vector<Entity>* entities){
    
	for(auto e : *entities){
		renderer.RenderEntity(e.renderEntity, &vao, &pointcount);
	}
}

void Display::Shutdown(){
	std::cout << "* Display\n";
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

	SDL_DestroyWindow(D_Window);

    D_Context = NULL;
	D_Window = NULL;
}

void Display::InitGL(){

	#ifndef __APPLE__
    glewExperimental = GL_TRUE;
	glewInit();
	#endif
    
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Display::SetViewport(int width, int height){
	GLfloat ratio;

	ratio = (GLfloat) width / (GLfloat) height;
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    SetPerspective(width, height, 60.0f, true);

	glMatrixMode(GL_MODELVIEW);
}

void Display::SetPerspective(int width, int height, float fov, bool usePerspective){
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
