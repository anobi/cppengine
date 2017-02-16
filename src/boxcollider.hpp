#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <glm/glm.hpp>
#include "EntityComponent.hpp"

class BoxCollider : public EntityComponent {

public:
	BoxCollider();
	~BoxCollider();

	bool IsColliding(BoxCollider $collider);

	void SetPosition(glm::fvec3 position);
	void SetPositionOffset(glm::fvec3 offset);
	glm::fvec3 GetPosition();

	void SetSize(glm::fvec3 size);
	void SetSizeOffset(glm::fvec3 offset);
	glm::fvec3 GetSize();

private:
	glm::fvec3 mPosition;
	glm::fvec3 mPositionOffeset;

	glm::fvec3 mSize;
	glm::fvec3 mSizeOffset;
	
protected:
};

#endif
