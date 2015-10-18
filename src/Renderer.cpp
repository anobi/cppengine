//GL headers for osx/linux
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Renderer.hpp"

Renderer::Renderer(){
}

Renderer::~Renderer(){
    
}

void Renderer::Init(){
}

void Renderer::RenderEntity(renderEntity_t entity){
    int bufferSize = entity.vertices.size() * 3; 
    GLfloat vertexBufferData[bufferSize];
    int i = 0;

    for(auto v : entity.vertices){
        vertexBufferData[i] = v.position.x;
        vertexBufferData[i+1] = v.position.y;
        vertexBufferData[i+2] = v.position.z;
        i +=3;
    }

    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexBufferData);
    glDrawArrays(GL_TRIANGLES, 0, bufferSize);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}
