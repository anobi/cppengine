#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "lib/Transform.hpp"
#include "Entity.hpp"

class EntityComponent {
public:
	EntityComponent() : mParent() {}
	virtual ~EntityComponent() {}

	virtual void Update() {}
	virtual void Render(Shader& shader, const glm::mat4 viewProjection){}
	virtual void AddToParent(Entity* parent) { mParent = parent; }

	inline Transform* GetTransform() { return mParent->GetTransform(); }
	inline const Transform& GetTransform() const { return *mParent->GetTransform(); }
	Entity* GetParent() { return this->mParent; }

private:
	Entity* mParent;
};

#endif
