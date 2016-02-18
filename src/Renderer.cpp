#include "Renderer.hpp"

Renderer::Renderer(){}
Renderer::~Renderer(){}

bool Renderer::Init(Display* display) {
	this->mDisplay = display;
	return true;
}

void Renderer::Render(Entity& entity) {
	entity.Render(mMainCamera->GetViewProjection());
}
