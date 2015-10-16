#include <iostream>
#include "Entity.hpp"

Entity::Entity(){
	location.Zero();
	alive = false;
}

Entity::~Entity(){
	
}

void Entity::Init(){

}

void Entity::Spawn(){
	
}

void Entity::Update(){
    //std::cout << "%s", renderEntity.vertices.size();
}

void Entity::Kill(){
	
}

void Entity::Remove(){
	
}

vector3<float> Entity::GetLocation(){
	return location;
}

void Entity::SetLocation(float x, float y, float z){
	location.Set(x, y, z);
}

//manual test cube mesh
void Entity::MakeTestEntity(){
	//left side
	vertex_t l1 = vertex_t();
	vertex_t l2 = vertex_t();
	vertex_t l3 = vertex_t();
	vertex_t l4 = vertex_t();
	l1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	l2.position = vector3<float>(0.5f, 0.0f, 0.0f);
	l3.position = vector3<float>(0.0f, 0.5f, 0.0f);
	l4.position = vector3<float>(0.5f, 0.5f, 0.0f);
	renderEntity.vertices.push_back(l1);
	renderEntity.vertices.push_back(l2);
	renderEntity.vertices.push_back(l3);
	renderEntity.vertices.push_back(l4);
	
	//front side
	vertex_t f1 = vertex_t();
	vertex_t f2 = vertex_t();
	vertex_t f3 = vertex_t();
	vertex_t f4 = vertex_t();
	f1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	f2.position = vector3<float>(0.0f, 0.0f, 0.5f);
	f3.position = vector3<float>(0.0f, 0.5f, 0.0f);
	f4.position = vector3<float>(0.0f, 0.5f, 0.5f);
	renderEntity.vertices.push_back(f1);
	renderEntity.vertices.push_back(f2);
	renderEntity.vertices.push_back(f3);
	renderEntity.vertices.push_back(f4);

	//right side
	vertex_t r1 = vertex_t();
	vertex_t r2 = vertex_t();
	vertex_t r3 = vertex_t();
	vertex_t r4 = vertex_t();
	r1.position = vector3<float>(0.0f, 0.0f, 0.5f);
	r2.position = vector3<float>(0.5f, 0.0f, 0.5f);
	r3.position = vector3<float>(0.0f, 0.5f, 0.5f);
	r4.position = vector3<float>(0.5f, 0.5f, 0.5f);
	renderEntity.vertices.push_back(r1);
	renderEntity.vertices.push_back(r2);
	renderEntity.vertices.push_back(r3);
	renderEntity.vertices.push_back(r4);
	
	//backside
	vertex_t b1 = vertex_t();
	vertex_t b2 = vertex_t();
	vertex_t b3 = vertex_t();
	vertex_t b4 = vertex_t();
	b1.position = vector3<float>(0.5f, 0.0f, 0.5f);
	b2.position = vector3<float>(0.5f, 0.0f, 0.0f);
	b3.position = vector3<float>(0.5f, 0.5f, 0.5f);
	b4.position = vector3<float>(0.5f, 0.5f, 0.0f);
	renderEntity.vertices.push_back(b1);
	renderEntity.vertices.push_back(b2);
	renderEntity.vertices.push_back(b3);
	renderEntity.vertices.push_back(b4);

	//topside
	vertex_t t1 = vertex_t();
	vertex_t t2 = vertex_t();
	vertex_t t3 = vertex_t();
	vertex_t t4 = vertex_t();
	t1.position = vector3<float>(0.0f, 0.5f, 0.0f);
	t2.position = vector3<float>(0.5f, 0.5f, 0.0f);
	t3.position = vector3<float>(0.0f, 0.5f, 0.5f);
	t4.position = vector3<float>(0.5f, 0.5f, 0.5f);
	renderEntity.vertices.push_back(t1);
	renderEntity.vertices.push_back(t2);
	renderEntity.vertices.push_back(t3);
	renderEntity.vertices.push_back(t4);

	//downside lol
	vertex_t d1 = vertex_t();
	vertex_t d2 = vertex_t();
	vertex_t d3 = vertex_t();
	vertex_t d4 = vertex_t();
	d1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	d2.position = vector3<float>(0.5f, 0.0f, 0.0f);
	d3.position = vector3<float>(0.0f, 0.0f, 0.5f);
	d4.position = vector3<float>(0.5f, 0.0f, 0.5f);
	renderEntity.vertices.push_back(b1);
	renderEntity.vertices.push_back(b2);
	renderEntity.vertices.push_back(b3);
	renderEntity.vertices.push_back(b4);
}
