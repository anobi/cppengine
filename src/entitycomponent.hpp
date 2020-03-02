#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <memory>

#include "transform.hpp"
#include "entity.hpp"

class EntityComponent {
public:
	EntityComponent() {}
	EntityComponent(std::shared_ptr<Entity> parent) { this->_parent = parent; }
	virtual ~EntityComponent() {}

	virtual void Update() {}
	virtual void AddToParent(std::shared_ptr<Entity> parent) { _parent = parent; }

	std::shared_ptr<Entity> GetParent() { return this->_parent; }
	void SetParent(std::shared_ptr<Entity> parent) { this->_parent = parent; }
	void SetName(const std::string name) { this->mComponentName = name; }

	const std::string GetName() const { return this->mComponentName; }
	Transform& GetTransform() { return _parent->GetTransform(); }


private:
	std::string mComponentName;
	std::shared_ptr<Entity> _parent;
};

#endif
