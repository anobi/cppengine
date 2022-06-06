#include <numeric>
#include <functional>
#include <algorithm>

#include "opengl.hpp"
#include "rendering/model_manager.hpp"

Rendering::ModelManager::ModelManager()
{
}

Rendering::ModelManager::~ModelManager()
{
}

void Rendering::ModelManager::Add(entityHandle_t entity)
{
    entitySlot_t resource = this->AllocateResource(entity);
}

void Rendering::ModelManager::LoadModel(entityHandle_t entity, std::vector<Vertex> vertices, std::vector<unsigned int> indexes)
{
    entitySlot_t resource = this->FindResource(entity);
    if (!resource.valid()) {
        return;
    }

    // TODO: Translate vertices into object space and the entity origin position in the center of the mesh
    // TODO: Also generate and add bounding boxes for each mesh entity

    // Calculate the center position by averaging out the mesh X, Y and Z positions
    // ---

    std::vector<float> x_positions;
    std::vector<float> y_positions;
    std::vector<float> z_positions;
    for (int i = 0; i < vertices.size(); i++) {
        x_positions.push_back(vertices[i].position.x);
        y_positions.push_back(vertices[i].position.y);
        z_positions.push_back(vertices[i].position.z);
    }

    auto minmax_x = std::minmax_element(x_positions.begin(), x_positions.end());
    auto minmax_y = std::minmax_element(y_positions.begin(), y_positions.end());
    auto minmax_z = std::minmax_element(z_positions.begin(), z_positions.end());

    float x_dist = std::abs(minmax_x.second[0] - minmax_x.first[0]);
    float y_dist = std::abs(minmax_y.second[0] - minmax_y.first[0]);
    float z_dist = std::abs(minmax_z.second[0] - minmax_z.first[0]);
    float radius = std::fmax(std::fmax(x_dist, y_dist), z_dist) / 2.0f;

    AABB aabb = AABB(
        glm::fvec3(minmax_x.first[0], minmax_y.first[0], minmax_z.first[0]),
        glm::fvec3(minmax_x.second[0], minmax_y.second[0], minmax_z.second[0])
    );
    this->AABBs[resource.slot] = aabb;

    glm::fvec3 midpoint = (aabb.min + aabb.max) / 2.0f;
    BoundingSphere bsphere = BoundingSphere(
        midpoint,
        radius
    );
    this->bounding_spheres[resource.slot] = bsphere;

    //// Calculate new vertex posisions offset around the origin
    //for (int i = 0; i < vertices.size(); i++) {
    //    vertices[i].position = glm::fvec3(
    //        vertices[i].position.x - midpoint.x,
    //        vertices[i].position.y - midpoint.y,
    //        vertices[i].position.z - midpoint.z
    //    );
    //}

    //// Translate the entity into the new center position
    //this->world->entity_manager->SetPosition(entity, midpoint);

    this->indices[resource.slot] = indexes.size();

    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &this->VAOs[resource.slot]);
    glBindVertexArray(this->VAOs[resource.slot]);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLuint), &indexes[0], GL_STATIC_DRAW);

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

void Rendering::ModelManager::Cleanup()
{
    for (int i = 0; i < this->_entities_top; i++) 
    {
        glDeleteVertexArrays(1, &this->VAOs[i]);
    }
    this->VAOs.Clear();
    this->indices.Clear();
    this->_entity_index.Clear();
}

entitySlot_t Rendering::ModelManager::AllocateResource(entityHandle_t entity)
{
    entitySlot_t resource;
    if (this->_entities_top < MAX_GAME_ENTITIES) {
        resource.slot = this->_entities_top;
        resource.entity = entity;
        this->_entity_index[this->_entities_top] = resource;
        this->_entities_top += 1;
    }
    return resource;
}

entitySlot_t Rendering::ModelManager::FindResource(entityHandle_t entity)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i].entity == entity) {
            return this->_entity_index[i];
        }
    }
    return entitySlot_t();
}

