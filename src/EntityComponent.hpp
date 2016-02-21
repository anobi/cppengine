#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "lib/Transform.hpp"
#include "Entity.hpp"

class EntityComponent {
public:
	EntityComponent() : mParent() {}
	virtual ~EntityComponent() {}

	virtual void Update() {}
	virtual void Render(Renderer& renderer){}
	virtual void AddToParent(Entity* parent) { mParent = parent; }

	inline std::string GetName() { return this->mComponentName; }
	inline void SetName(const std::string name) { this->mComponentName = name; }

	inline Transform* GetTransform() { return mParent->GetTransform(); }

	Entity* GetParent() { return this->mParent; }

private:
	std::string mComponentName;
	Entity* mParent;
};

#endif
