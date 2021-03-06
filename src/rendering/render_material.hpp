#ifndef R_RENDER_MATERIAL_H
#define R_RENDER_MATERIAL_H

#include "../opengl.hpp"
#include "../shader.hpp"

class RenderMaterial
{
public:
    RenderMaterial() {};
    ~RenderMaterial() {};

    void Setup(GLuint* material_slot, const int width, const int height, const void* data);
    void SetUniforms(const int value);
    void Bind(const Shader* shader);
    void Unbind();
    void Cleanup();

    const Shader* boundShader = 0;

    GLuint diffuseMap   = 0;
    GLuint specularMap  = 0;
    GLuint normalMap    = 0;
    GLuint alphaMap     = 0;
};

#endif // R_RENDER_MATERIAL_H