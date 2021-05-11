#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "transform.hpp"

class Camera {
public:
    Camera() {
        this->transform = Transform();
        this->projection = glm::mat4();
    }

    Camera(float fov, float near, float far) {
        this->fov = fov;
        this->near = near;
        this->far = far;
        this->transform = Transform();
        this->projection = glm::mat4();
    }

    ~Camera() {};

    inline void SetAspectRatio(float aspectRatio) {
        this->projection = glm::perspective(
            glm::radians(this->fov),
            aspectRatio,
            this->near,
            this->far
        );
    }

    inline glm::fmat4 GetView() {
        return glm::lookAt(
            this->transform.GetPosition(),
            this->transform.GetPosition() + this->transform.GetDirection(),
            this->transform.GetUp()
        );
    }

    inline glm::fmat4 GetProjection() { return projection; }
    inline glm::fmat4 GetViewProjection() { return projection * GetView(); }

    Transform transform;
    glm::fmat4 projection;

    float fov   = 45.0f;
    float near  = 0.1f;
    float far   = 100.0f;
};

#endif
