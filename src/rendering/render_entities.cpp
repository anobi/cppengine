#include "../opengl.hpp"
#include "render_entities.hpp"

void RenderEntities::Add(entityHandle_t entity)
{
    this->_entities[entity.slot] = entity;
    if (entity.slot > this->_entities_top) {
        this->_entities_top = entity.slot + 1;
    }
    else {
        this->_entities_top += 1;
    }
}

void RenderEntities::LoadModel(entityHandle_t entity, const std::vector<Vertex> vertices, const std::vector<unsigned int> indices)
{
    this->indices[entity.slot] = indices.size();
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &this->VAOs[entity.slot]);
    glBindVertexArray(this->VAOs[entity.slot]);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RenderEntities::Cleanup()
{
    for (int i = 0; i < this->_entities_top; i++) 
    {
        glDeleteVertexArrays(1, &this->VAOs[i]);
    }
    this->VAOs.Clear();
    this->indices.Clear();
    this->_entities.Clear();
}

