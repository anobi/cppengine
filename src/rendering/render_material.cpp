#include "render_material.hpp"

void RenderMaterial::Setup(GLuint* material_slot, const int width, const int height, const void* data)
{
    glGenTextures(1, material_slot);
    glBindTexture(GL_TEXTURE_2D, *material_slot);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void RenderMaterial::SetUniforms(const int value)
{
    assert(this->boundShader);

    int useDiffuse = this->diffuseMap > 0;
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(this->boundShader->_uniforms.diffuse, 0 * value);
    glUniform1i(this->boundShader->_uniforms.use_diffuse, useDiffuse * value);
    glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

    int useSpecular = this->specularMap > 0;
    glActiveTexture(GL_TEXTURE0 + 1);
    glUniform1i(this->boundShader->_uniforms.specular, 1 * value);
    glUniform1i(this->boundShader->_uniforms.use_specular, useSpecular * value);
    glBindTexture(GL_TEXTURE_2D, this->specularMap);

    int useNormal = this->normalMap > 0;
    glActiveTexture(GL_TEXTURE0 + 2);
    glUniform1i(this->boundShader->_uniforms.normal, 2 * value);
    glUniform1i(this->boundShader->_uniforms.use_normal, useNormal * value);
    glBindTexture(GL_TEXTURE_2D, this->normalMap);

    int useAlpha = this->alphaMap > 0;
    glActiveTexture(GL_TEXTURE0 + 3);
    glUniform1i(this->boundShader->_uniforms.alpha, 3 * value);
    glUniform1i(this->boundShader->_uniforms.use_alpha, useAlpha * value);
    glBindTexture(GL_TEXTURE_2D, this->alphaMap);
}

void RenderMaterial::Bind(const Shader* shader)
{
    this->boundShader = shader;
    SetUniforms(1);
}

void RenderMaterial::Unbind() 
{
    SetUniforms(0);
    glActiveTexture(GL_TEXTURE0);
    this->boundShader = 0;
}

void RenderMaterial::Cleanup()
{
    glDeleteTextures(1, &this->diffuseMap);
    glDeleteTextures(1, &this->specularMap);
    glDeleteTextures(1, &this->normalMap);
    glDeleteTextures(1, &this->alphaMap);
}
