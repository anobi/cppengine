#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <memory>

#include "transform.hpp"
#include "entity.hpp"

class EntityComponent {
public:
	EntityComponent() {}
	virtual ~EntityComponent() {}

	virtual void Update() {}
	virtual void Render(Renderer &renderer){}
	virtual void AddToParent(Entity* parent) { mParent = parent; }

	Entity* GetParent() { return this->mParent; }
	void SetParent(Entity* parent) { this->mParent = parent; }
	void SetName(const std::string name) { this->mComponentName = name; }

	const std::string GetName() const { return this->mComponentName; }
	Transform& GetTransform() { return mParent->GetTransform(); }


private:
	std::string mComponentName;
	Entity* mParent;
};

#endif
