#include <fstream>
#include <sstream>
#include <string.h>
#include <cstdio>
#include <algorithm>

#include "configuration.hpp"
#include "shader.hpp"

constexpr unsigned int MAX_PATH_LENGTH = 4096;
constexpr unsigned int MAX_UNIFORM_LENGTH = 4096;
constexpr unsigned int MAX_SHADER_LENGTH = 32768;

Shader::Shader(const char* fileName)
{
    printf("* Loading shader: %s \n", fileName);
    this->name = (char*) fileName;

    program = glCreateProgram();

    char vertex_shader_file[MAX_PATH_LENGTH];
    char fragment_shader_file[MAX_PATH_LENGTH];
    snprintf(vertex_shader_file, MAX_PATH_LENGTH, "%s.vert", fileName);
    snprintf(fragment_shader_file, MAX_PATH_LENGTH, "%s.frag", fileName);

    std::string vertex_shader_data = this->ReadFile(vertex_shader_file);
    if (vertex_shader_data.size() == 0) {
        printf("No vertex shader data!");
        return;
    }

    std::string fragment_shader_data = this->ReadFile(fragment_shader_file);
    if (fragment_shader_data.size() == 0) {
        printf("No fragment shader data!");
        return;
    }

    shaders[0] = CreateShader(vertex_shader_data.c_str(), GL_VERTEX_SHADER);
    shaders[1] = CreateShader(fragment_shader_data.c_str(), GL_FRAGMENT_SHADER);

    for (unsigned int i = 0; i < NUM_SHADERS; i++) {
        glAttachShader(program, shaders[i]);
    }

    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "normal");
    glBindAttribLocation(program, 2, "tangent");
    glBindAttribLocation(program, 3, "bitangent");
    glBindAttribLocation(program, 4, "texCoord");

    glLinkProgram(program);
    glValidateProgram(program);

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

    _uniforms.diffuse = uniforms[7];
    _uniforms.specular = uniforms[8];
    _uniforms.normal = uniforms[9];
    _uniforms.alpha = uniforms[12];
    _uniforms.use_diffuse = uniforms[13];
    _uniforms.use_specular = uniforms[14];
    _uniforms.use_normal = uniforms[16];
    _uniforms.use_alpha = uniforms[18];



    unsigned int loc = POINTLIGHT_UNIFORM_OFFSET;
    for (unsigned int i = 0; i < MAX_POINTLIGHTS; i++)
    {
        char point_light_position[MAX_UNIFORM_LENGTH];
        char point_light_color[MAX_UNIFORM_LENGTH];
        char point_light_intensity[MAX_UNIFORM_LENGTH];
        char point_light_radius[MAX_UNIFORM_LENGTH];
        char point_light_cutoff[MAX_UNIFORM_LENGTH];

        snprintf(point_light_position,  MAX_UNIFORM_LENGTH, "pointLights[%i].position",     i);
        snprintf(point_light_color,     MAX_UNIFORM_LENGTH, "pointLights[%i].color",        i);
        snprintf(point_light_intensity, MAX_UNIFORM_LENGTH, "pointLights[%i].intensity",    i);
        snprintf(point_light_radius,    MAX_UNIFORM_LENGTH, "pointLights[%i].radius",       i);
        snprintf(point_light_cutoff,    MAX_UNIFORM_LENGTH, "pointLights[%i].cutoff",       i);

        uniforms[loc + 0] = glGetUniformLocation(program, point_light_position);
        uniforms[loc + 1] = glGetUniformLocation(program, point_light_color);
        uniforms[loc + 2] = glGetUniformLocation(program, point_light_intensity);
        uniforms[loc + 3] = glGetUniformLocation(program, point_light_radius);
        uniforms[loc + 4] = glGetUniformLocation(program, point_light_cutoff);

        loc += POINTLIGHT_UNIFORMS;
    }

    unsigned int dloc = DLIGHT_UNIFORM_OFFSET;
    for (unsigned int i = 0; i < MAX_DLIGHTS; i++)
    {
        char directional_light_position[MAX_UNIFORM_LENGTH];
        char directional_light_color[MAX_UNIFORM_LENGTH];
        char directional_light_intensity[MAX_UNIFORM_LENGTH];

        snprintf(directional_light_position,    MAX_UNIFORM_LENGTH, "directionalLights[%i].position",   i);
        snprintf(directional_light_color,       MAX_UNIFORM_LENGTH, "directionalLights[%i].color",      i);
        snprintf(directional_light_intensity,   MAX_UNIFORM_LENGTH, "directionalLights[%i].intensity",  i);

        uniforms[dloc + 0] = glGetUniformLocation(program, directional_light_position);
        uniforms[dloc + 1] = glGetUniformLocation(program, directional_light_color);
        uniforms[dloc + 2] = glGetUniformLocation(program, directional_light_intensity);

        dloc += DLIGHT_UNIFORMS;
    }
}

Shader::~Shader()
{
    this->Cleanup();
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


GLuint Shader::CreateShader(const char* source, unsigned int type)
{
    GLuint shader = glCreateShader(type);
    GLint length = strlen(source);

    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);
    GetShaderStatus(shader);

    return shader;
}

void Shader::Bind()
{
    glUseProgram(this->program);
}

/*
** Helpers
**/

std::string Shader::ReadFile(const char* fileName)
{
    std::ostringstream oss;

#ifdef _WIN32 
    std::string shaders_dir = "shaders/";
#else
    std::string shaders_dir = "/shaders/";
#endif

    oss << Configuration::Get().workingDirectory << shaders_dir << fileName;
    std::string filePath = oss.str();

    std::string content;
    std::fstream stream(filePath, std::ios::in);

    if (!stream.is_open()) {
        std::cerr << "  !! ERROR: Unable to read shader file: " << filePath << std::endl;
        return "";
    }

    std::string line = "";
    while (!stream.eof()) {
        std::getline(stream, line);
        content.append(line + "\n");
    }

    stream.close();

    if (content.size() == 0) {
        std::cerr << "  !! ERROR: Unable to read shader data: " << filePath << std::endl;
        return "";
    }
    return content;
}

void Shader::GetShaderStatus(GLuint shader)
{
    int shaderLogLength = 0;
    int programLogLength = 0;

    GLint shaderResult = GL_FALSE;
    GLint programResult = GL_FALSE;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderResult);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLogLength);

    glGetProgramiv(shader, GL_COMPILE_STATUS, &programResult);
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &programLogLength);

    GLchar shaderError[512] = "";
    glGetShaderInfoLog(shader, shaderLogLength, NULL, &shaderError[0]);

    GLchar programError[512] = "";
    glGetShaderInfoLog(shader, programLogLength, NULL, &programError[0]);

    if (shaderLogLength > 1) 
    {
        std::string shaderErrorMessage = "";
        for (auto c : shaderError)
        {
            shaderErrorMessage += c;
        }
            
        if (!shaderErrorMessage.empty())
        {
            printf("  !! Shader error: \n%s\n", shaderErrorMessage);
        }
    }

    if (programLogLength > 1) 
    {
        std::string programErrorMessage = "";
        for (auto c : programError)
            programErrorMessage += c;

        if (!programErrorMessage.empty())
        {
            printf("  !! Program error: \n%s\n", programErrorMessage);
        } 
    }
}
