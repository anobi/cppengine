#include "frustum.hpp"

Frustum::Frustum(Camera* camera) 
{
	this->_camera = camera;
}

Frustum::~Frustum()
{
	this->_camera = nullptr;
}

void Frustum::Update(glm::fmat4 viewProjection) 
{
}

bool Frustum::Intersects(const BoundingSphere sphere)
{
	bool intersects = true;

	for (int i = 0; i < this->_planes.size(); i++)
	{
		glm::fvec4 p = this->_planes[i];
		glm::fvec3 s = sphere.center;
		float r = sphere.radius;
		if (p.x * s.x + p.y * s.y + p.z * s.z + p.w <= -r)
		{
			intersects = false;
		}
	}

	return intersects;
}
bool Frustum::Intersects(const BoundingBox box)
{
	return true;
}
