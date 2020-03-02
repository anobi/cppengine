#include "gl_light.hpp"

void GL::GLPointLight::UpdateShaderAttributes(std::shared_ptr<Shader> shader) {
	int loc = this->uniform_location;
	glUniform3fv(shader->uniforms[loc + 0], 1, &this->_light->GetTransform().GetPosition()[0]);
	glUniform3fv(shader->uniforms[loc + 1], 1, &this->_light->GetColor()[0]);
	glUniform1f(shader->uniforms[loc + 2], this->_light->GetIntensity());
	glUniform1f(shader->uniforms[loc + 3], this->_light->GetRadius());
	glUniform1f(shader->uniforms[loc + 4], this->_light->GetCutoff());
}

void GL::GLDirectionalLight::UpdateShaderAttributes(std::shared_ptr<Shader> shader)
{
	int loc = this->uniform_location;
	glUniform3fv(shader->uniforms[loc + 0], 1, &this->_light->GetTransform().GetPosition()[0]);
	glUniform3fv(shader->uniforms[loc + 1], 1, &this->_light->GetColor()[0]);
	glUniform1f(shader->uniforms[loc + 2], this->_light->GetIntensity());
}
