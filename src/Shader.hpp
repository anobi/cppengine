#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "lib/OpenGL.hpp"

class Shader {

public:
	Shader();
	Shader(const std::string filename);
	~Shader();

	void Bind();
	GLuint Program();

private:
	std::string ReadFile(std::string filename);
	std::string GetShaderStatus(GLuint program);
	GLuint program;
};

#endif