#include "Renderer.hpp"

Renderer::Renderer(){}
Renderer::~Renderer(){}

bool Renderer::Init(Display* display) {

	this->mDisplay = display;
	mShader = Shader("default");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}


void Renderer::Render(Entity& entity) {
	entity.Render(mShader, mMainCamera->GetViewProjection());
}
