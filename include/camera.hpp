#ifndef __CPPE_CAMERA_H__
#define __CPPE_CAMERA_H__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "transform.hpp"
#include "frustum.hpp"

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
        this->view_frustum = Frustum();
    }

    ~Camera() {};

    inline void SetAspectRatio(float aspectRatio) {
        this->aspect_ratio = aspectRatio;
        this->Update();
    }

    inline glm::fmat4 GetView() { return this->view; }
    inline glm::fmat4 GetProjection() { return this->projection; }
    inline glm::fmat4 GetViewProjection() { return GetProjection() * GetView(); }

    void Update() {
        // Update view
        glm::fvec3 p = this->transform.GetPosition();
        glm::fvec3 d = this->transform.GetDirection();
        glm::fvec3 u = glm::normalize(this->transform.GetUp());
        this->view = glm::lookAt(p, p + d, u);

        // Update projection
        this->projection = glm::perspective(
            glm::radians(this->fov),
            this->aspect_ratio,
            this->near,
            this->far
        );

        // Update frustum planes
        glm::fmat4 A = glm::transpose(this->GetViewProjection());
        this->view_frustum.UpdatePlanes(A);
    }

    Transform transform;
    Frustum view_frustum;

    glm::fmat4 view;
    glm::fmat4 projection;

    float aspect_ratio = 0.0f;
    float fov   = 60.0f;
    float near  = 0.001f;
    float far   = 100.0f;
};

#endif  // __CPPE_CAMERA_H__
