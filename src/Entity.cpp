#include "Entity.hpp"

Entity::Entity(const std::string& mesh, const std::string& shader, const std::string& texture) {
	this->mMesh = new Mesh(mesh);
	this->mShader = new Shader(shader);
	this->mTransform = new Transform();
	this->mTexture = new Texture(texture);
}

Entity::~Entity() {
	this->mMesh->~Mesh();
	this->mShader->~Shader();
	this->mTexture->~Texture();
}

void Entity::Update(Camera& camera) {
	this->mTexture->Bind();
	this->mShader->Bind();
	this->mShader->Update(*mTransform, camera);
	this->mMesh->Draw();
}