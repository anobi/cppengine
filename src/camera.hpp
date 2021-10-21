#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "transform.hpp"

struct Plane {
    glm::fvec3 n;
    float d;

    Plane() { 
        this->n = glm::fvec3(); 
        this->d = 0.0f;
    }

    Plane(float a, float b, float c, float d, bool normalize) {
        float l = glm::sqrt(a * a + b * b + c * c);
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
    }

    ~Camera() {};

    inline void SetAspectRatio(float aspectRatio) {
        this->aspect_ratio = aspectRatio;
        this->Update();
    }

    inline glm::fmat4 GetView() { return this->view; }
    inline glm::fmat4 GetProjection() { return this->projection;  }
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
        glm::fmat4 A = this->projection * this->view;

        // Left
        this->frustum_planes[0] = Plane(
            A[3][0] + A[0][0],
            A[3][1] + A[0][1],
            A[3][2] + A[0][2],
            A[3][3] + A[0][3],
            true
        );
        // Right
        this->frustum_planes[1] = Plane(
            A[3][0] - A[0][0],
            A[3][1] - A[0][1],
            A[3][2] - A[0][2],
            A[3][3] - A[0][3],
            true
        );
        // Top
        this->frustum_planes[1] = Plane(
            A[3][0] - A[1][0],
            A[3][1] - A[1][1],
            A[3][2] - A[1][2],
            A[3][3] - A[1][3],
            true
        );
        // Bottom
        this->frustum_planes[1] = Plane(
            A[3][0] + A[1][0],
            A[3][1] + A[1][1],
            A[3][2] + A[1][2],
            A[3][3] + A[1][3],
            true
        );
        // Near
        this->frustum_planes[1] = Plane(
            A[3][0] + A[2][0],
            A[3][1] + A[2][1],
            A[3][2] + A[2][2],
            A[3][3] + A[2][3],
            true
        );
        // Far
        this->frustum_planes[1] = Plane(
            A[3][0] - A[2][0],
            A[3][1] - A[2][1],
            A[3][2] - A[2][2],
            A[3][3] - A[2][3],
            true
        );
    }

    Transform transform;
    glm::fmat4 view;
    glm::fmat4 projection;
    Plane frustum_planes[6];

    float aspect_ratio = 0.0f;
    float fov   = 60.0f;
    float near  = 0.001f;
    float far   = 100.0f;
};

#endif
