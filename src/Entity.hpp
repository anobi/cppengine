#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>

#include "lib/Transform.hpp"

/////////////////
// E N T I T Y //
/////////////////

class EntityComponent;
class Shader;
class Texture;
class Renderer;
class Camera;

class Entity {
public:
	Entity() : mTransform() {}
    ~Entity();

	void Update();
	void Render(Renderer& renderer);

	void AddComponent(EntityComponent* component);
	EntityComponent* GetComponent(const std::string componentName);
	inline Transform* GetTransform() { return &mTransform; }

private:
	std::string mEntityName;
	std::vector<EntityComponent*> mComponents;
	Transform mTransform;
};

#endif
