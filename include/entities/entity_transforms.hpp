#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../constants.hpp"
#include "../containers/array.hpp"
#include "../entities/entity_handle.hpp"

class World;

class EntityTransforms {
public:
    EntityTransforms() {};
    ~EntityTransforms() {};

    void Add(entityHandle_t entity);
    void Update(std::vector<entityHandle_t> entities, glm::fmat4 view_projectio);
    void SetPosition(entityHandle_t entity, glm::fvec3 position);
    void SetRotation(entityHandle_t entity, glm::fvec3 rotation);
    void SetScale(entityHandle_t entity, glm::fvec3 scale);

    glm::fvec3 GetPosition(entityHandle_t entity);
    glm::fvec3 GetRotation(entityHandle_t entity);
    glm::fvec3 GetScale(entityHandle_t entity);
    glm::fmat4 GetModel(entityHandle_t entity);
    

    unsigned int _entities_top = 0;
    Array<entitySlot_t, MAX_GAME_ENTITIES> _entity_index;
    entitySlot_t AllocateResource(entityHandle_t entity);
    entitySlot_t FindResource(entityHandle_t entity);


    Array<glm::fvec3, MAX_GAME_ENTITIES> positions;
    Array<glm::fvec3, MAX_GAME_ENTITIES> rotations;
    Array<glm::fvec3, MAX_GAME_ENTITIES> scales;
    Array<glm::fmat4, MAX_GAME_ENTITIES> model_matrices;
    Array<glm::fmat4, MAX_GAME_ENTITIES> mvp_matrices;
    Array<glm::fmat3, MAX_GAME_ENTITIES> normal_matrices;

    std::vector<entitySlot_t> _model_update_queue;
    void UpdateModels();

    const glm::fvec3 _up = glm::fvec3(0.0f, 1.0f, 0.0f);
    const glm::fvec3 _direction = glm::fvec3(0.0f, 0.0f, -1.0f);
};

#endif // __GAME_ENTITIES_H__