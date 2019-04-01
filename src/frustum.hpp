#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <vector>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "types.hpp"

class Frustum {
public:
	Frustum(Camera* camera);
	~Frustum();

	void Update(glm::fmat4 viewProjection);

	bool Intersects(const BoundingSphere sphere);
	bool Intersects(const BoundingBox box);

private:

	Camera* _camera;
	std::vector<glm::fvec4> _planes;
};

#endif