

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "Shader.hpp"

Shader::Shader(){}

Shader::Shader(const std::string &fileName) {
	program = glCreateProgram();

	shaders[0] = CreateShader(ReadFile(fileName + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(ReadFile(fileName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normal");

	glLinkProgram(program);
	std::string lError = GetShaderStatus(program);

	glValidateProgram(program);
	std::string vError = GetShaderStatus(program);

	uniforms[0] = glGetUniformLocation(program, "diffuse");
	uniforms[1] = glGetUniformLocation(program, "modelViewProjection");
	uniforms[2] = glGetUniformLocation(program, "wNormal");
}

Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteShader(program);
}


GLuint Shader::CreateShader(const std::string &source, unsigned int type) {
	GLuint shader = glCreateShader(type);	
	
	const char* shaderSrc = source.c_str();
	GLint length[1];
	length[0] = source.length();
	glShaderSource(shader, 1, &shaderSrc, length);
	glCompileShader(shader);

	std::string vError = GetShaderStatus(shader);

	return shader;
}

void Shader::Bind() {
	glUseProgram(this->program);
}

void Shader::Update(const Transform &transform, const Camera &camera) {
	glm::mat4 modelViewProjection = transform.GetModelViewProjection(camera);
	glm::mat4 normal = transform.GetModel();

	glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &modelViewProjection[0][0]);
	glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &normal[0][0]);
	glUniform3f(uniforms[2], 0.0f, 0.0f, 1.0f);
}

/*
** Helpers
**/

std::string Shader::ReadFile(const std::string &fileName) {

	std::string filePath;

#ifdef _WIN32
	filePath = "../shaders/" + fileName;
#else
	filePath = "shaders/" + fileName;
#endif

	std::string content;
	std::ifstream stream(filePath, std::ios::in);

	if (!stream.is_open()) {
		std::cerr << "Cannot read file " << filePath << std::endl;
		return "";
	}

	std::string line = "";
	while (!stream.eof()) {
		std::getline(stream, line);
		content.append(line + "\n");
	}

	stream.close();
	return content;
}

std::string Shader::GetShaderStatus(GLuint shader) {

	int logLength = 0;
	GLint result = GL_FALSE;
	std::string errorMessage = "";

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> shaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);

	if (shaderError.size() > 1) {
		for (auto c : shaderError) {
			errorMessage += c;
		}
	}
	return errorMessage;
}