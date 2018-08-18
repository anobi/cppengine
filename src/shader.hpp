#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "opengl.hpp"

class Shader
{

public:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 128;

	static const unsigned int MAX_POINTLIGHTS = 5;
	static const unsigned int POINTLIGHT_UNIFORM_OFFSET = 32;
	static const unsigned int POINTLIGHT_UNIFORMS = 5;

	static const unsigned int MAX_DLIGHTS = 3;
	static const unsigned int DLIGHT_UNIFORM_OFFSET = 64;
	static const unsigned int DLIGHT_UNIFORMS = 3;

	Shader(const std::string filename);
	~Shader();

	std::string name;

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];

	void Bind();
	void Cleanup();

private:
	GLuint CreateShader(const std::string source, unsigned int type);
	std::string ReadFile(const std::string filename);
	void GetShaderStatus(GLuint program);
};

#endif
