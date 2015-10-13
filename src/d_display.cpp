#include <iostream>
#include <math.h>
#include <SDL2/SDL_opengl.h>

//GL headers for osx/linux
#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "d_display.hpp"

SDL_GLContext D_Context; 

Display::Display(){
    D_Window = 0;
    D_Renderer = NULL;
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
    
	glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
	glRotatef(0.1f,0.2f,0.0f,0.2f);    // Rotate The cube around the Y axis

    //
    // IT A CUBE
    //
    
    //left
	glColor3f(0.5f,0.5f,0.5f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.5f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.5f, 0.0f );
        glVertex3f( 0.5f, 0.5f, 0.0f );
	glEnd();

    //front
	glColor3f(0.8f,0.8f,0.8f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.5f );
        glVertex3f( 0.0f, 0.5f, 0.0f );
        glVertex3f( 0.0f, 0.5f, 0.5f );
	glEnd();

    //right
	glColor3f(0.2f,0.2f,0.2f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.0f, 0.0f, 0.5f );
        glVertex3f( 0.5f, 0.0f, 0.5f );
        glVertex3f( 0.0f, 0.5f, 0.5f );
        glVertex3f( 0.5f, 0.5f, 0.5f );
	glEnd();

    //back
	glColor3f(1.0f,1.0f,1.0f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.5f, 0.0f, 0.5f );
        glVertex3f( 0.5f, 0.0f, 0.0f );
        glVertex3f( 0.5f, 0.5f, 0.5f );
        glVertex3f( 0.5f, 0.5f, 0.0f );
	glEnd();

    //floor
	glColor3f(0.6f,0.6f,0.6f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.5f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.5f );
        glVertex3f( 0.5f, 0.0f, 0.5f );
	glEnd();

    //ceiling
	glColor3f(0.1f,0.1f,0.1f); 
	glBegin( GL_TRIANGLE_STRIP );
        glVertex3f( 0.0f, 0.5f, 0.0f );
        glVertex3f( 0.5f, 0.5f, 0.0f );
        glVertex3f( 0.0f, 0.5f, 0.5f );
        glVertex3f( 0.5f, 0.5f, 0.5f );
	glEnd();
	
	SDL_GL_SwapWindow(D_Window);
}

void Display::Shutdown(){
	std::cout << "* Display\n";
	SDL_DestroyWindow(D_Window);
	D_Window = NULL;
}

void Display::InitGL(){
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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
