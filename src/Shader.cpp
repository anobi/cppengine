#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Shader.hpp"

Shader::Shader(const std::string fileName) : EntityComponent() {
	program = glCreateProgram();

	shaders[0] = CreateShader(ReadFile(fileName + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(ReadFile(fileName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 1, "position");
	glBindAttribLocation(program, 2, "normal");
	glBindAttribLocation(program, 3, "tangent");
	glBindAttribLocation(program, 4, "bitangent");
	glBindAttribLocation(program, 5, "texCoord");

	glLinkProgram(program);
	std::string lError = GetShaderStatus(program);

	glValidateProgram(program);
	std::string vError = GetShaderStatus(program);

	uniforms[0] = glGetUniformLocation(program, "ModelMatrix");
	uniforms[1] = glGetUniformLocation(program, "ViewMatrix");
	uniforms[2] = glGetUniformLocation(program, "ProjectionMatrix");
	uniforms[3] = glGetUniformLocation(program, "CameraPosition");

	//Texture maps
	uniforms[4] = glGetUniformLocation(program, "AlbedoMap");
	uniforms[5] = glGetUniformLocation(program, "NormalMap");

	unsigned int loc = LIGHT_UNIFORM_OFFSET;
	for (unsigned int i = 0; i < MAX_LIGHTS; i++) {

		std::stringstream light;
		light << "Lights[" << i << "]";

		//LOOKS BETTER ALIGNED LOL
		uniforms[loc + 0] = glGetUniformLocation(program, (light.str() + ".direction").c_str());
		uniforms[loc + 1] = glGetUniformLocation(program, (light.str() + ".position").c_str());
		uniforms[loc + 2] = glGetUniformLocation(program, (light.str() + ".color").c_str());
		uniforms[loc + 3] = glGetUniformLocation(program, (light.str() + ".intensity").c_str());
		uniforms[loc + 4] = glGetUniformLocation(program, (light.str() + ".radius").c_str());
		uniforms[loc + 5] = glGetUniformLocation(program, (light.str() + ".cutoff").c_str());

		loc += NUM_LIGHT_UNIFORMS;
	}
}

Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteShader(program);
}


GLuint Shader::CreateShader(const std::string source, unsigned int type) {
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

void Shader::UpdateUniforms(Transform &transform, Renderer &renderer) {

	//need to get modelview without projection
	glm::fmat4 model = transform.GetModel();
	glm::fmat4 view = renderer.GetCamera()->GetView();
	glm::fmat4 projection = renderer.GetCamera()->GetProjection();
	glm::fvec3 eyePos = renderer.GetCamera()->GetPosition();

	glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &projection[0][0]);

	glUniform3fv(uniforms[3], 1, &eyePos[0]);

	glUniform1i(uniforms[4], 0); //colormap
	glUniform1i(uniforms[5], 1); //normal map

	auto lights = renderer.GetLights();
	unsigned int loc = LIGHT_UNIFORM_OFFSET;
	for (unsigned int i = 0; i < lights.size() && i < MAX_LIGHTS; i++) {

		std::shared_ptr<PointLight> light = std::dynamic_pointer_cast<PointLight>(lights[i]);
		glm::fvec3 lPos = light->GetTransform().GetPosition();

		glUniform3fv(uniforms[loc + 0], 1, &glm::fvec3(0.5f, 1.0f, -1.0f)[0]);
		glUniform3fv(uniforms[loc + 1], 1, &lPos[0]);
		glUniform3fv(uniforms[loc + 2], 1, &light->GetColor()[0]);
		glUniform1f(uniforms[loc + 3], light->GetIntensity());
		glUniform1f(uniforms[loc + 4], light->GetRadius());
		glUniform1f(uniforms[loc + 5], light->GetCutoff());

		loc += NUM_LIGHT_UNIFORMS;
	}
}

/*
** Helpers
**/

std::string Shader::ReadFile(const std::string fileName) {

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
