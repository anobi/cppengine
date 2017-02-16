#include "BoxCollider.hpp"

BoxCollider::BoxCollider() {

}

BoxCollider::~BoxCollider() {

}

bool BoxCollider::IsColliding(BoxCollider $collider)
{
	return false;
}

void BoxCollider::SetPosition(glm::fvec3 position)
{
	this->mPosition = position;
}

void BoxCollider::SetPositionOffset(glm::fvec3 offset)
{
	this->mPositionOffeset = offset;
}

glm::fvec3 BoxCollider::GetPosition()
{
	return this->mPosition;
}

void BoxCollider::SetSize(glm::fvec3 size)
{
	this->mSize = size;
}

void BoxCollider::SetSizeOffset(glm::fvec3 offset)
{
	this->mSizeOffset = offset;
}

glm::fvec3 BoxCollider::GetSize()
{
	return glm::fvec3();
}
