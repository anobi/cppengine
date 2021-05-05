#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// TODO: Shouldn't this be translation or something?
// Also, this might be too complicated. 
// Just save the position & rotation in objects and m ake some helpers where they are needed.
class Transform {
public:
    Transform() {
        this->position = glm::fvec3(0.0f, 0.0f, 0.0f);
        this->rotation = glm::fvec3(0.0f, 0.0f, 0.0f);
        this->scale = glm::fvec3(1.0f, 1.0f, 1.0f);
        this->up = glm::fvec3(0.0f, 1.0f, 0.0f);
        this->direction = glm::fvec3(0.0f, 0.0f, -1.0f);
    }

    glm::fmat4 GetModel() {
        glm::fmat4 positionMatrix = glm::translate(glm::fmat4(1.0f), position);
        glm::fmat4 scaleMatrix = glm::scale(glm::fmat4(1.0f), scale);
        glm::fmat4 rotationX = glm::rotate(glm::fmat4(1.0f), rotation.x, glm::fvec3(1.0, 0.0, 0.0));
        glm::fmat4 rotationY = glm::rotate(glm::fmat4(1.0f), rotation.y, glm::fvec3(0.0, 1.0, 0.0));
        glm::fmat4 rotationZ = glm::rotate(glm::fmat4(1.0f), rotation.z, glm::fvec3(0.0, 0.0, 1.0));
        glm::fmat4 rotationMatrix = rotationX * rotationY * rotationZ;

        return positionMatrix * rotationMatrix * scaleMatrix;
    }

    glm::fmat4 GetModelViewProjection(const glm::fmat4& viewProjection) {
        glm::fmat4 model = GetModel();
        return viewProjection * model;
    }

    inline glm::fvec3& GetPosition() { return this->position; }
    inline glm::fvec3& GetRotation() { return this->rotation; }
    inline glm::fvec3& GetScale() { return this->scale; }
    inline glm::fvec3 GetRight() {
        glm::fvec3 rotation = this->GetRotation();
        return glm::fvec3(
            sin(rotation.x - 3.14f / 2.0f),
            0,
            cos(rotation.x - 3.14f / 2.0f)
        );
    }

    inline glm::fvec3 GetUp() {
        return glm::cross(GetRight(), this->GetDirection());
    }

    inline glm::fvec3 GetDirection() {
        return glm::fvec3(
            cos(rotation.y) * sin(rotation.x),
            sin(rotation.y),
            cos(rotation.y) * cos(rotation.x));
    }

    inline void SetPosition(const glm::fvec3& position) { this->position = position; }
    inline void SetRotation(const glm::fvec3& rotation) { this->rotation = rotation; }
    inline void SetScale(const glm::fvec3& scale) { this->scale = scale; }
    inline void LookAt(const glm::fvec3& target) {
        glm::mat4 translation = glm::lookAt(this->position, target, this->up);
    }

private:
    glm::fvec3 position;
    glm::fvec3 rotation;
    glm::fvec3 scale;
    glm::fvec3 direction;
    glm::fvec3 up;
};

#endif
