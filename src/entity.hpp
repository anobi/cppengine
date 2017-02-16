#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <memory>
#include <vector>

#include "transform.hpp"

/////////////////
// E N T I T Y //
/////////////////

class Renderer;
class Camera;
class EntityComponent;
class Entity;

typedef std::shared_ptr<EntityComponent> EntityComponentRef;
typedef std::shared_ptr<Entity> EntityRef;

class Entity {
public:
	Entity() {}
	Entity(const std::string name) {
		this->mEntityName = name;
	}
    ~Entity();

	void Update();
	void Render(Renderer &renderer);

	void SetName(const std::string name) { this->mEntityName = name; };
	const std::string GetName() const { return this->mEntityName; }

	EntityComponentRef AddComponent(EntityComponentRef component);
	EntityComponentRef GetComponent(const std::string componentName);

	Transform& GetTransform() { return mTransform; }

private:
	std::string mEntityName;
	std::vector<EntityComponentRef> mComponents;
	Transform mTransform;
};

#endif
