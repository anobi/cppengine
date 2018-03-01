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
	GetShaderStatus(program);

	glValidateProgram(program);
	GetShaderStatus(program);

	// Detach and clean up the shaders after linking them to the program
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	uniforms[0] = glGetUniformLocation(program, "ModelMatrix");
	uniforms[1] = glGetUniformLocation(program, "ViewMatrix");
	uniforms[2] = glGetUniformLocation(program, "ProjectionMatrix");
	uniforms[3] = glGetUniformLocation(program, "NormalMatrix");
	uniforms[4] = glGetUniformLocation(program, "Resolution");
	uniforms[5] = glGetUniformLocation(program, "CameraPosition");
	uniforms[6] = glGetUniformLocation(program, "Time");

	//Texture maps
	uniforms[7] = glGetUniformLocation(program, "diffuseMap");
	uniforms[8] = glGetUniformLocation(program, "specularMap");
	uniforms[9] = glGetUniformLocation(program, "normalMap");
	uniforms[10] = glGetUniformLocation(program, "heightMap");
	uniforms[11] = glGetUniformLocation(program, "emissiveMap");
	uniforms[12] = glGetUniformLocation(program, "alphaMap");

	uniforms[13] = glGetUniformLocation(program, "use_diffuseMap");
	uniforms[14] = glGetUniformLocation(program, "use_specularMap");
	uniforms[15] = glGetUniformLocation(program, "use_heightMap");
	uniforms[16] = glGetUniformLocation(program, "use_normalMap");
	uniforms[17] = glGetUniformLocation(program, "use_emissiveMap");
	uniforms[18] = glGetUniformLocation(program, "use_alphaMap");

	unsigned int loc = POINTLIGHT_UNIFORM_OFFSET;
	for (unsigned int i = 0; i < MAX_POINTLIGHTS; i++)
	{
		std::stringstream pointlight;
		pointlight << "pointLights[" << i << "]";

		//LOOKS BETTER ALIGNED LOL
		uniforms[loc + 0] = glGetUniformLocation(program, (pointlight.str() + ".position").c_str());
		uniforms[loc + 1] = glGetUniformLocation(program, (pointlight.str() + ".color").c_str());
		uniforms[loc + 2] = glGetUniformLocation(program, (pointlight.str() + ".intensity").c_str());
		uniforms[loc + 3] = glGetUniformLocation(program, (pointlight.str() + ".radius").c_str());
		uniforms[loc + 4] = glGetUniformLocation(program, (pointlight.str() + ".cutoff").c_str());

		loc += POINTLIGHT_UNIFORMS;
	}

	unsigned int dloc = DLIGHT_UNIFORM_OFFSET;
	for (unsigned int i = 0; i < MAX_DLIGHTS; i++)
	{
		std::stringstream directionalLight;
		directionalLight << "directionalLights[" << i << "]";

		//LOOKS BETTER ALIGNED LOL
		uniforms[dloc + 0] = glGetUniformLocation(program, (directionalLight.str() + ".position").c_str());
		uniforms[dloc + 1] = glGetUniformLocation(program, (directionalLight.str() + ".color").c_str());
		uniforms[dloc + 2] = glGetUniformLocation(program, (directionalLight.str() + ".intensity").c_str());

		dloc += DLIGHT_UNIFORMS;
	}
}

Shader::~Shader()
{
	//this->Cleanup();
}

void Shader::Cleanup()
{
	glUniform1i(glGetUniformLocation(program, "diffuseMap"), 0);
	glUniform1i(glGetUniformLocation(program, "specularMap"), 0);
	glUniform1i(glGetUniformLocation(program, "normalMap"), 0);
	glUniform1i(glGetUniformLocation(program, "heightMap"), 0);
	glUniform1i(glGetUniformLocation(program, "emissiveMap"), 0);
	glUniform1i(glGetUniformLocation(program, "alphaMap"), 0);

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
