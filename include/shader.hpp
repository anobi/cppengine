#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "opengl.hpp"

typedef struct {
    int diffuse;
    int use_diffuse;

    int specular;
    int use_specular;

    int normal;
    int use_normal;

    int alpha;
    int use_alpha;
} shader_uniforms_t;


class Shader
{

public:
    static const unsigned int NUM_SHADERS = 2;
    static const unsigned int NUM_UNIFORMS = 128;

    static const unsigned int MAX_POINTLIGHTS = 5;
    static const unsigned int POINTLIGHT_UNIFORM_OFFSET = 32;
    static const unsigned int POINTLIGHT_UNIFORMS = 5;

    static const unsigned int MAX_DLIGHTS = 3;
    static const unsigned int DLIGHT_UNIFORM_OFFSET = 64;
    static const unsigned int DLIGHT_UNIFORMS = 3;

    Shader() {};
    Shader(const char* filename);
    ~Shader() {};

    std::string name;

    GLuint program;
    GLuint shaders[NUM_SHADERS];
    GLuint uniforms[NUM_UNIFORMS];
    shader_uniforms_t _uniforms;

    void Bind();
    void Cleanup();

private:
    GLuint CreateShader(const char* source, unsigned int type);
    std::string ReadFile(const char* filename);
    void GetShaderStatus(GLuint program);
};

#endif
