#include "opengl.hpp"
#include "gl_renderer.hpp"


void GL::GLRenderer::Shutdown()
{
}

bool GL::GLRenderer::Init()
{
	return true;
}

void GL::GLRenderer::Render(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader)
{
	shader->Bind();
	//
	// Update dynamic lights
	//
	for (unsigned int i = 0; i < this->_dynamicLights.size(); i++)
	{
		this->_dynamicLights[i]->UpdateShaderAttributes(shader);
	}

	//
	// Update and render models
	//
	for (int i = 0; i < this->_renderEntities.size(); i++)
	{
		// Update uniforms
		glm::fvec2 resolution = this->GetResolution();
		glm::fvec3 eyePos = camera->GetPosition();
		glm::fmat4 model = this->_renderEntities[i]->GetModelMatrix();
		glm::fmat4 view = camera->GetView();
		glm::fmat4 projection = camera->GetProjection();
		glm::fmat3 normalMatrix = glm::inverse(glm::fmat3(model));

		glUniformMatrix4fv(shader->uniforms[0], 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(shader->uniforms[1], 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(shader->uniforms[2], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix3fv(shader->uniforms[3], 1, GL_FALSE, &normalMatrix[0][0]);

		glUniform2fv(shader->uniforms[4], 1, &resolution[0]);
		glUniform3fv(shader->uniforms[5], 1, &eyePos[0]);

		// Time
		glUniform1i(shader->uniforms[6], this->GetTick());

		// Render
		this->_renderEntities[i]->Draw();
	}

	// Post processing pass here?
}