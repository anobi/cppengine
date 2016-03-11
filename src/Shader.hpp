#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "lib/OpenGL.hpp"

#include "EntityComponent.hpp"
#include "Renderer.hpp"

class Shader : public EntityComponent {

public:
	Shader(const std::string filename);
	Shader() : EntityComponent() {};
	~Shader();

	void Bind();
	void UpdateUniforms(Transform &transform, Renderer &renderer);
	void Render(Renderer &renderer){}

private:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 128;
	static const unsigned int MAX_LIGHTS = 5;
	static const unsigned int LIGHT_UNIFORM_OFFSET = 6;
	static const unsigned int NUM_LIGHT_UNIFORMS = 6;

	GLuint CreateShader(const std::string source, unsigned int type);
	std::string ReadFile(const std::string filename);
	std::string GetShaderStatus(GLuint program);

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];
};

#endif
