#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "transform.hpp"
#include "frustum.hpp"

struct Plane {
    glm::fvec3 n;
    float d;

    Plane() { 
        this->n = glm::fvec3(); 
        this->d = 0.0f;
    }

    Plane(float a, float b, float c, float d, bool normalize) {
        float l = 1.0f;
        if (normalize) {
            // float l = glm::sqrt((a * a) + (b * b) + (c * c));
            float l = glm::length(glm::fvec3(a, b, c));
        }
        this->n = glm::fvec3(a / l, b / l, c / l);
        this->d = d / l;
    }
};

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

#endif
