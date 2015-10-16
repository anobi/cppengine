#include <GL/gl.h>
#include "Renderer.hpp"

Renderer::Renderer(){
    this->display = nullptr;
}

Renderer::~Renderer(){
    
}

void Renderer::Init(Display *display){
    this->display = display; 
}

void Renderer::RenderEntity(renderEntity_t entity){
    int bufferSize = entity.vertices.size(); 
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
