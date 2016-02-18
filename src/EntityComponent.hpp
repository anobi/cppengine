#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "lib/Transform.hpp"
#include "Entity.hpp"

class EntityComponent {
public:
	EntityComponent() : mParent() {}
	virtual ~EntityComponent() {}

	virtual void Update() {}
	virtual void Render(const glm::mat4 viewProjection){}
	virtual void AddToParent(Entity* parent) { mParent = parent; }

	inline std::string GetName() { return this->mComponentName; }
	inline void SetName(const std::string name) { this->mComponentName = name; }

	inline Transform* GetTransform() { return mParent->GetTransform(); }
	inline const Transform& GetTransform() const { return *mParent->GetTransform(); }

	Entity* GetParent() { return this->mParent; }

protected:
	std::string mComponentName;

private:
	Entity* mParent;
};

#endif
