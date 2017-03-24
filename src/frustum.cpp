#include "frustum.hpp"

Frustum::Frustum(Camera* camera) 
{
	this->camera = camera;
}

Frustum::~Frustum()
{
	this->camera = nullptr;
}

void Frustum::Update(glm::fmat4 viewProjection) 
{
}

bool Frustum::Intersects(const boundingSphere sphere)
{
	return true;
}