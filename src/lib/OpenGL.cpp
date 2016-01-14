#include "OpenGL.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

std::string ReadFile(const char* filePath) {
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

GLuint GLUtils::LoadShader(const char* vertex_shader_path, const char* fragment_shader_path) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderStr = ReadFile(vertex_shader_path);
	std::string fragmentShaderStr = ReadFile(fragment_shader_path);
	const char* vertexShaderSrc = vertexShaderStr.c_str();
	const char* fragmentShaderSrc = fragmentShaderStr.c_str();

	int logLength = 0;
	GLint result = GL_FALSE;

	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertexShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderError[0]);
	std::cout << &vertexShaderError[0] << std::endl;

	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragmentShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderError[0]);
	std::cout << &fragmentShaderError[0] << std::endl;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
