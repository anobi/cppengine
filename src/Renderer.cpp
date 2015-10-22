//GL headers for osx/linux
#include <vector>
#include <memory>

#include "lib/OpenGL.hpp"
#include "Renderer.hpp"

Renderer::Renderer(){
}

Renderer::~Renderer(){
    
}

void Renderer::Init(){
}

void Renderer::RenderEntity(renderEntity_t entity){




    std::vector<vector3<GLfloat> > vertexData;
    std::vector<vector3<GLfloat> > colorData;
    for (auto v : entity.vertices) {
        vertexData.push_back(vector3<GLfloat>(v.position.x
                                              , v.position.y
                                              , v.position.z));
        colorData.push_back(vector3<GLfloat>(v.color.x
                                             , v.color.y
                                             , v.color.z));
    }

	int pointcount = 0;
    pointcount = vertexData.size() * 3;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER
                 , vertexData.size() * sizeof(vector3<GLfloat>)
				 , vertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

    /*GLuint color_vbo = 0;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER
                 , colorData.size() * sizeof(vector3<GLfloat>)
				 , colorData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);*/

	glDrawArrays(GL_TRIANGLES, 0, pointcount);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}
