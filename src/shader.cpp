#include <fstream>
#include <sstream>

#include "shader.hpp"

Shader::Shader(const std::string fileName)
{
	this->name = fileName;

	program = glCreateProgram();

	shaders[0] = CreateShader(ReadFile(fileName + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(ReadFile(fileName + ".frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindAttribLocation(program, 2, "tangent");
	glBindAttribLocation(program, 3, "bitangent");
	glBindAttribLocation(program, 4, "texCoord");

	glLinkProgram(program);
	std::string lError = GetShaderStatus(program);

	glValidateProgram(program);
	std::string vError = GetShaderStatus(program);

	uniforms[0] = glGetUniformLocation(program, "ModelMatrix");
	uniforms[1] = glGetUniformLocation(program, "ViewMatrix");
	uniforms[2] = glGetUniformLocation(program, "ProjectionMatrix");
	uniforms[3] = glGetUniformLocation(program, "NormalMatrix");
	uniforms[4] = glGetUniformLocation(program, "Resolution");
	uniforms[5] = glGetUniformLocation(program, "CameraPosition");
	uniforms[6] = glGetUniformLocation(program, "Time");

	//Texture maps
	uniforms[7] = glGetUniformLocation(program, "material.diffuse");
	uniforms[8] = glGetUniformLocation(program, "material.specular");
	uniforms[9] = glGetUniformLocation(program, "material.normal");
	uniforms[10] = glGetUniformLocation(program, "material.height");
	uniforms[11] = glGetUniformLocation(program, "material.emissive");

	uniforms[13] = glGetUniformLocation(program, "material.useDiffuseMap");
	uniforms[14] = glGetUniformLocation(program, "material.useSpecularMap");
	uniforms[15] = glGetUniformLocation(program, "material.useHeightMap");
	uniforms[16] = glGetUniformLocation(program, "material.useNormalMap");
	uniforms[17] = glGetUniformLocation(program, "material.useEmissiveMap");

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

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteShader(program);
}


GLuint Shader::CreateShader(const std::string source, unsigned int type)
{
	GLuint shader = glCreateShader(type);	
	
	const char* shaderSrc = source.c_str();
	GLint length[1];
	length[0] = source.length();
	glShaderSource(shader, 1, &shaderSrc, length);
	glCompileShader(shader);

	std::string vError = GetShaderStatus(shader);

	return shader;
}

void Shader::Bind()
{
	glUseProgram(this->program);
}

/*
** Helpers
**/

std::string Shader::ReadFile(const std::string fileName)
{
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

	int shaderLogLength = 0;
	int programLogLength = 0;

	GLint shaderResult = GL_FALSE;
	GLint programResult = GL_FALSE;

	std::string errorMessage = "";

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderResult);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLogLength);

	glGetProgramiv(shader, GL_COMPILE_STATUS, &programResult);
	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &programLogLength);

	std::vector<GLchar> shaderError((shaderLogLength > 1) ? shaderLogLength : 1);
	glGetShaderInfoLog(shader, shaderLogLength, NULL, &shaderError[0]);

	std::vector<GLchar> programError((programLogLength > 1) ? programLogLength : 1);
	glGetShaderInfoLog(shader, programLogLength, NULL, &programError[0]);

	if (shaderError.size() > 1) {
		for (auto c : shaderError) {
			errorMessage += c;
		}
        std::cerr << errorMessage << std::endl;
        std::cerr << "Shader error: " << errorMessage << std::endl;
	}

	if (programError.size() > 1) {
		for (auto c : programError) {
			errorMessage += c;
		}
        std::cerr << "Program error: " << errorMessage << std::endl;
	}

	return errorMessage;
}
