#include "Entity.hpp"

Entity::Entity(){
	location.Zero();
	alive = false;
	renderEntity = nullptr;
}

Entity::~Entity(){
	
}

void Entity::Spawn(){
	
}

void Entity::Update(){
	
}

void Entity::Kill(){
	
}

void Entity::Remove(){
	
}

vector3 Entity::GetLocation(){
	return location;
}

void Entity::SetLocation(float x, float y, float z){
	location.Set(x, y, z);
}
