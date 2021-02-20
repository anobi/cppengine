#include "mesh.hpp"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::shared_ptr<Texture>> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->SetupMesh();
}

Mesh::~Mesh() 
{
    //glDeleteBuffers(1, &this->VBO);
    //glDeleteVertexArrays(1, &this->VAO);
}

void Mesh::SetupMesh() 
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, bitangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader) 
{
    for(int i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        int uloc = 0;
        int use_uloc = 0;
        switch(this->textures[i]->type) {
        case DIFFUSE_MAP:
            uloc = shader->_uniforms.diffuse;
            use_uloc = shader->_uniforms.use_diffuse;
            break;
        case SPECULAR_MAP:
            uloc = shader->_uniforms.specular;
            use_uloc = shader->_uniforms.use_specular;
            break;
        case NORMAL_MAP:
            uloc = shader->_uniforms.normal;
            use_uloc = shader->_uniforms.use_normal;
            break;
        case ALPHA_MAP:
            uloc = shader->_uniforms.alpha;
            use_uloc = shader->_uniforms.use_alpha;
            break;
        default:
            break;
        }

        glUniform1i(uloc, i);
        glUniform1i(use_uloc, 1);
        glBindTexture(GL_TEXTURE_2D, this->textures[i]->id);
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*) 0);

    // Reset the texture map uniforms so they don't leak in to wrong meshes
    for(int i = 0; i < this->textures.size(); i++)
    {
        int uloc = 0;
        int use_uloc = 0;
        switch (this->textures[i]->type) {
        case DIFFUSE_MAP:
            uloc = shader->_uniforms.diffuse;
            use_uloc = shader->_uniforms.use_diffuse;
            break;
        case SPECULAR_MAP:
            uloc = shader->_uniforms.specular;
            use_uloc = shader->_uniforms.use_specular;
            break;
        case NORMAL_MAP:
            uloc = shader->_uniforms.normal;
            use_uloc = shader->_uniforms.use_normal;
            break;
        case ALPHA_MAP:
            uloc = shader->_uniforms.alpha;
            use_uloc = shader->_uniforms.use_alpha;
            break;
        default:
            break;
        }

        glUniform1i(uloc, 0);
        glUniform1i(use_uloc, 0);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(0);
}

void Mesh::Cleanup()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    for(int i = 0; i < this->textures.size(); i++)
    {
        glDeleteTextures(1, &this->textures[i]->id);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);
}
