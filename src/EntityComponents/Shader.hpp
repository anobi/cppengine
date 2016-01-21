#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "../lib/OpenGL.hpp"

#include "../EntityComponent.hpp"

class Shader : public EntityComponent {

public:
	Shader(const std::string &filename);
	Shader();
	~Shader();

	void Bind();
	virtual void UpdateUniforms(const Transform& transform, const Camera& camera) const;

private:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 3;

	GLuint CreateShader(const std::string &source, unsigned int type);
	std::string ReadFile(const std::string &filename);
	std::string GetShaderStatus(GLuint program);

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];
};

#endif