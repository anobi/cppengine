#ifndef CPPE_FRUSTUM_H
#define CPPE_FRUSTUM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


 enum class FrustumIntersectResult {
    OUT         = 0,
    INTERSECT   = 1,
    IN          = 2
};


class Frustum {
public:
    Frustum() {
    }

    void UpdatePlanes(const glm::fmat4 view_projection);

    FrustumIntersectResult SphereIntersect(const glm::fvec3 sphere_position, const float sphere_radius);

    glm::fvec4 planes[6];
    bool normalize = true;
};

#endif