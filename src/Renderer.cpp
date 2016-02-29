#include "Renderer.hpp"

Renderer::Renderer(){

}
Renderer::~Renderer(){
	mMainCamera = NULL;
}

bool Renderer::Init(Display &display) {
	this->mDisplay = &display;
	return true;
}

void Renderer::Render(EntityRef entity) {
	entity->Render(*this);
}