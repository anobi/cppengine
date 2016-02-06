#include "Renderer.hpp"

Renderer::Renderer(){}
Renderer::~Renderer(){}

bool Renderer::Init(Display* display) {

	this->mDisplay = display;
	mShader = Shader("default");

	return true;
}


void Renderer::Render(Entity& entity) {
	entity.Render(mShader, mMainCamera->GetViewProjection());
}
