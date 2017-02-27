#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "opengl.hpp"

class Shader
{

public:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 128;
	static const unsigned int MAX_LIGHTS = 5;
	static const unsigned int LIGHT_UNIFORM_OFFSET = 32;
	static const unsigned int NUM_LIGHT_UNIFORMS = 6;

	Shader(const std::string filename);
	~Shader();

	std::string name;

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];

	void Bind();

private:
	GLuint CreateShader(const std::string source, unsigned int type);
	std::string ReadFile(const std::string filename);
	std::string GetShaderStatus(GLuint program);

};

#endif
