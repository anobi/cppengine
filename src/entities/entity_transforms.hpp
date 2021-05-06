#ifndef __GAME_ENTITIES_H__
#define __GAME_ENTITIES_H__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../constants.hpp"
#include "../containers/array.hpp"
#include "../entities/entity_handle.hpp"

class Entity;
class World;

class EntityTransforms {
public:
    EntityTransforms() {};
    ~EntityTransforms() {};

    Array<glm::fvec3, MAX_GAME_ENTITIES> positions;
    Array<glm::fvec3, MAX_GAME_ENTITIES> rotations;
    Array<glm::fvec3, MAX_GAME_ENTITIES> scales;
    Array<glm::fmat4, MAX_GAME_ENTITIES> model_matrices;
    Array<glm::fmat4, MAX_GAME_ENTITIES> mvp_matrices;
    Array<glm::fmat3, MAX_GAME_ENTITIES> normal_matrices;

    void Add(entityHandle_T entity);
    void SetPosition(entityHandle_T entity, glm::fvec3 position);
    void SetRotation(entityHandle_T entity, glm::fvec3 rotation);
    void SetScale(entityHandle_T entity, glm::fvec3 scale);
    void Update(glm::fmat4 view_projection);

private:
    unsigned int _entities_top = 0;
    Array<entityHandle_T, MAX_GAME_ENTITIES> _entities;
    const glm::fvec3 _up = glm::fvec3(0.0f, 1.0f, 0.0f);
    const glm::fvec3 _direction = glm::fvec3(0.0f, 0.0f, -1.0f);
};

#endif // __GAME_ENTITIES_H__