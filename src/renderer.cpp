#include "opengl.hpp"
#include "renderer.hpp"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

bool Renderer::Init()
{
	std::shared_ptr<Shader> defaultShader = std::make_shared<Shader>(Shader("default"));
	this->AddShader(defaultShader);
	return true;
}

std::shared_ptr<Shader> Renderer::GetShader(const std::string name)
{
	std::shared_ptr<Shader> shader = nullptr;

	for(int i = 0; i < this->shaders.size(); i++)
	{
		if(this->shaders[i]->name == name)
		{
			shader = shaders[i];
			break;
		}
	}

	return shader;
}

void Renderer::Render()
{

	// Render shadowmaps
	
	// Render entities
	this->currentShader = this->GetShader("default");
	for(int i = 0; i < models.size(); i++)
	{
		// Bind shader
		currentShader->Bind();

		// Update uniforms

		glm::fmat4 model = models[i]->GetTransform().GetModel();
		glm::fmat4 view = this->GetCamera()->GetView();
		glm::fmat4 projection = this->GetCamera()->GetProjection();
		glm::fvec2 resolution = this->GetResolution();
		glm::fvec3 eyePos = this->GetCamera()->GetPosition();
		glm::fmat3 normalMatrix = glm::inverse(glm::fmat3(model));

		glUniformMatrix4fv(this->currentShader->uniforms[0], 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(this->currentShader->uniforms[1], 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(this->currentShader->uniforms[2], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix3fv(this->currentShader->uniforms[3], 1, GL_FALSE, &normalMatrix[0][0]);

		glUniform2fv(this->currentShader->uniforms[4], 1, &resolution[0]);
		glUniform3fv(this->currentShader->uniforms[5], 1, &eyePos[0]);

		// Time
		glUniform1i(this->currentShader->uniforms[6], this->GetTick());

		// Lights
		unsigned int loc = 32; // Light uniform offset
		for (unsigned int i = 0; i < this->lights.size(); i++)
		{
			std::shared_ptr<PointLight> light = std::dynamic_pointer_cast<PointLight>(this->lights[i]);
			glm::fvec3 lPos = light->GetTransform().GetPosition();

			glUniform3fv(this->currentShader->uniforms[loc + 0], 1, &glm::fvec3(0.5f, 1.0f, -1.0f)[0]);
			glUniform3fv(this->currentShader->uniforms[loc + 1], 1, &lPos[0]);
			glUniform3fv(this->currentShader->uniforms[loc + 2], 1, &light->GetColor()[0]);
			glUniform1f(this->currentShader->uniforms[loc + 3], light->GetIntensity());
			glUniform1f(this->currentShader->uniforms[loc + 4], light->GetRadius());
			glUniform1f(this->currentShader->uniforms[loc + 5], light->GetCutoff());

			loc += 6; // Number of light uniforms
		}

		// Render
		models[i]->Render(this->currentShader);
	}

	// Post processing pass here?
}
