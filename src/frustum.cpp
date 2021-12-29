#include "frustum.hpp"


void Frustum::UpdatePlanes(const glm::fmat4 view_projection) {
    glm::fmat4 A = view_projection;
    this->planes[0] = glm::fvec4(
        A[3][0] + A[0][0],
        A[3][1] + A[0][1],
        A[3][2] + A[0][2],
        A[3][3] + A[0][3]
    );
    this->planes[1] = glm::fvec4(
        A[3][0] - A[0][0],
        A[3][1] - A[0][1],
        A[3][2] - A[0][2],
        A[3][3] - A[0][3]
    );
    this->planes[2] = glm::fvec4(
        A[3][0] - A[1][0],
        A[3][1] - A[1][1],
        A[3][2] - A[1][2],
        A[3][3] - A[1][3]
    );
    this->planes[3] = glm::fvec4(
        A[3][0] + A[1][0],
        A[3][1] + A[1][1],
        A[3][2] + A[1][2],
        A[3][3] + A[1][3]
    );
    this->planes[4] = glm::fvec4(
        A[3][0] + A[2][0],
        A[3][1] + A[2][1],
        A[3][2] + A[2][2],
        A[3][3] + A[2][3]
    );
    this->planes[5] = glm::fvec4(
        A[3][0] - A[2][0],
        A[3][1] - A[2][1],
        A[3][2] - A[2][2],
        A[3][3] - A[2][3]
    );

    if (this->normalize) {
        for (int i = 0; i < 5; i++) {
            this->planes[i] = glm::normalize(this->planes[i]);
        }
    }

}

FrustumIntersectResult Frustum::SphereIntersect(const glm::fvec3 sphere_position, const float sphere_radius) {
    for (int i = 0; i < 5; i++) {
        glm::fvec3 plane = glm::fvec3(this->planes[i].x, this->planes[i].y, this->planes[i].z);
        float normal = this->planes[i].w;
        float dist = glm::dot(sphere_position, plane) + normal + sphere_radius;
        if (dist < 0) {
            return FrustumIntersectResult::OUT;
        }
        else if (dist == 0) {
            return FrustumIntersectResult::INTERSECT;
        }
    }

    return FrustumIntersectResult::IN;
}