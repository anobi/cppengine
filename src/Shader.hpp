#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "lib/OpenGL.hpp"
#include "lib/Camera.hpp"
#include "lib/Transform.hpp"

class Shader {

public:
	Shader(const std::string &filename);
	~Shader();

	void Bind();
	void Update(const Transform &transform, const Camera &camera);
	GLuint Program();

private:
	static const int NUM_SHADERS = 2;
	static const int NUM_UNIFORMS = 3;

	GLuint CreateShader(std::string &source, unsigned int type);
	std::string ReadFile(std::string &filename);
	std::string GetShaderStatus(GLuint program);


	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];
};

#endif