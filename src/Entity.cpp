#include "Entity.hpp"
#include "EntityComponent.hpp"

Entity::~Entity() {
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		mComponents[i]->~EntityComponent();
	}
}

void Entity::Render(Renderer &renderer){
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		mComponents[i]->Render(renderer);
	}
}

void Entity::Update(){}

EntityComponentRef Entity::AddComponent(EntityComponentRef component) {
	component->SetParent(this);
	mComponents.push_back(component);
	return mComponents.back();
}

EntityComponentRef Entity::GetComponent(const std::string name) {
	EntityComponentRef component = nullptr;
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		if (mComponents[i]->GetName() == name) {
			component = mComponents[i];
			break;
		}
	}
	return component;
}
