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

	glBindAttribLocation(program, 1, "position");
	glBindAttribLocation(program, 2, "normal");
	glBindAttribLocation(program, 3, "texCoord");

	glLinkProgram(program);
	std::string lError = GetShaderStatus(program);

	glValidateProgram(program);
	std::string vError = GetShaderStatus(program);

	uniforms[0] = glGetUniformLocation(program, "Normal");
	uniforms[1] = glGetUniformLocation(program, "ModelViewProjection");
	uniforms[2] = glGetUniformLocation(program, "LightDirection");
	uniforms[3] = glGetUniformLocation(program, "LightPosition");
	uniforms[4] = glGetUniformLocation(program, "LightColor");
	uniforms[5] = glGetUniformLocation(program, "LightIntensity");
	uniforms[6] = glGetUniformLocation(program, "LightMaxDistance");
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

void Shader::UpdateUniforms(const Transform& transform, Renderer& renderer) const {

	glm::mat4 modelViewProjection = transform.GetModelViewProjection(renderer.GetCamera().GetViewProjection());
	glm::mat4 normal = transform.GetModel();

	glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &normal[0][0]);
	glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &modelViewProjection[0][0]);
	glUniform3f(uniforms[2], 0.5f, 1.0f, -1.0f);

	if (renderer.GetLights().size() > 0) {
		Light* light = renderer.GetLights()[0];

		if (light != NULL) {
			glm::fvec3* lPos = light->GetTransform()->GetPosition();
			glUniform3f(uniforms[3], lPos->x, lPos->y, lPos->z);
			glUniform3f(uniforms[4], light->color.r, light->color.g, light->color.b);
			glUniform1f(uniforms[5], light->intensity);
			glUniform1f(uniforms[6], light->maxDistance);
		}
	}
}

/*
** Helpers
**/

std::string Shader::ReadFile(const std::string &fileName) {

	std::string filePath;

	filePath = "shaders/" + fileName;

	std::string content;
	std::fstream stream(filePath, std::ios::in);

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
        std::cerr << errorMessage << std::endl;
	}
	return errorMessage;
}
