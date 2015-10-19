//GL headers for osx/linux
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "Renderer.hpp"

Renderer::Renderer(){
}

Renderer::~Renderer(){
    
}

void Renderer::Init(){
}

void Renderer::RenderEntity(renderEntity_t entity){
    constexpr int bufferSize = sizeof(entity.vertices);

    GLfloat vertexBufferData[72];
	GLfloat colorBufferData[72];
    int i = 0;

    for(auto v : entity.vertices){
        vertexBufferData[i] = v.position.x;
        vertexBufferData[i+1] = v.position.y;
        vertexBufferData[i+2] = v.position.z;
        colorBufferData[i] = v.color.x;
        colorBufferData[i+1] = v.color.y;
        colorBufferData[i+2] = v.color.z;
        i +=3;
    }

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData)
				 , vertexBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 72);
	glDisableVertexAttribArray(0);
}
