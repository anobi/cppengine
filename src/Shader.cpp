

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "Shader.hpp"

Shader::Shader(){}

Shader::Shader(const std::string fileName) {

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string filePath;

#ifdef _WIN32
	filePath = "../shaders/" + fileName;
#else
	filePath = "shaders/" + fileName;
#endif

	std::string vShader = ReadFile(filePath + ".vert");
	std::string fShader = ReadFile(filePath + ".frag");
	const char* vertexShaderSrc = vShader.c_str();
	const char* fragmentShaderSrc = fShader.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	std::string vError = GetShaderStatus(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	std::string fError = GetShaderStatus(fragmentShader);

	GLuint program = glCreateProgram();
	std::string pError = GetShaderStatus(program);

	glAttachShader(program, vertexShader);
	glBindAttribLocation(program, 0, "mvp");

	glAttachShader(program, fragmentShader);
	glBindFragDataLocation(program, 0, "inColor");

	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->program = program;
}

Shader::~Shader() {
	glDeleteShader(program);
}

void Shader::Bind() {
	glUseProgram(this->program);
}

GLuint Shader::Program() {
	return this->program;
}

std::string Shader::ReadFile(std::string filePath) {
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