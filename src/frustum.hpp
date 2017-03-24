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

	// Need structs for bounding boxes & spheres
	bool Intersects(const boundingSphere sphere);

private:

	Camera* camera;
	std::vector<glm::fmat4x3> planes;

};

#endif