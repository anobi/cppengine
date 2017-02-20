#include <fstream>
#include <sstream>

#include "shader.hpp"

Shader::Shader(const std::string fileName) : EntityComponent() {

	this->SetName("Shader");

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
	uniforms[7] = glGetUniformLocation(program, "AlbedoMap");
	uniforms[8] = glGetUniformLocation(program, "NormalMap");
	uniforms[9] = glGetUniformLocation(program, "HeightMap");
	uniforms[10] = glGetUniformLocation(program, "RoughnessMap");
	uniforms[11] = glGetUniformLocation(program, "OcclusionMap");
	uniforms[12] = glGetUniformLocation(program, "MetallicMap");

	uniforms[13] = glGetUniformLocation(program, "UseHeightMap");

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

void Shader::Render(Renderer & renderer) {
	this->Bind();
	this->UpdateUniforms(GetTransform(), renderer);
}

void Shader::UpdateUniforms(Transform &transform, Renderer &renderer) {

	//need to get modelview without projection
	glm::fmat4 model = transform.GetModel();
	glm::fmat4 view = renderer.GetCamera()->GetView();
	glm::fmat4 projection = renderer.GetCamera()->GetProjection();
	glm::fvec2 resolution = renderer.GetResolution();
	glm::fvec3 eyePos = renderer.GetCamera()->GetPosition();
	glm::fmat3 normalMatrix = glm::inverse(glm::fmat3(model));

	glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix3fv(uniforms[3], 1, GL_FALSE, &normalMatrix[0][0]);

	glUniform2fv(uniforms[4], 1, &resolution[0]);
	glUniform3fv(uniforms[5], 1, &eyePos[0]);

	glUniform1i(uniforms[6], renderer.GetTick());
	glUniform1i(uniforms[7], 0); //colormap
	glUniform1i(uniforms[8], 1); //normal map
	glUniform1i(uniforms[9], 2); //height map

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
