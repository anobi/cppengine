#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "lib/Camera.hpp"
#include "lib/Transform.hpp"

/////////////////
// E N T I T Y //
/////////////////

class Entity {
public:
    Entity();
    Entity(const std::string& mesh, const std::string& shader, const std::string& texture);
    ~Entity();

	void Update(Camera& camera);

	inline glm::vec3 *Position() { return this->mTransform->Position(); }
	inline glm::vec3 *Rotation() { return this->mTransform->Rotation(); }
	inline glm::vec3 *Scale() { return this->mTransform->Scale(); }

	inline void SetPosition(glm::vec3 &position) { this->mTransform->SetPosition(position); }
	inline void SetRotation(glm::vec3 &rotation) { this->mTransform->SetRotation(rotation); }
	inline void SetScale(glm::vec3 &scale) { this->mTransform->SetScale(scale); }

private:
	Mesh* mMesh;
	Shader* mShader;
	Transform* mTransform;
	Texture* mTexture;
};

#endif
