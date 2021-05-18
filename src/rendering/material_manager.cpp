#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "../configuration.hpp"
#include "material_manager.hpp"



materialHandle_t Rendering::MaterialManager::Add(const char* name)
{
    materialHandle_t existing = this->FindResource(name);
    if (existing.valid()) {
        return existing;
    }

    materialHandle_t material = materialHandle_t();
    material.id = this->_next_id;
    this->_next_id += 1;

    materialSlot_t resource = this->AllocateResource(material);
    this->names[resource.slot] = name;

    return material;
}

void Rendering::MaterialManager::Setup(materialHandle_t material, TextureType_e type, const int width, const int height, const void* data)
{
    materialSlot_t resource = this->FindResource(material);
    if (resource.valid()) 
    {
        GLuint* slot;
        switch (type)
        {
        case DIFFUSE:
            slot = &this->diffuse_maps[resource.slot];
            break;
        case SPECULAR:
            slot = &this->specular_maps[resource.slot];
            break;
        case NORMAL:
            slot = &this->normal_maps[resource.slot];
            break;
        case ALPHA:
            slot = &this->alpha_maps[resource.slot];
            break;
        default:
            return;
        }

        glGenTextures(1, slot);
        glBindTexture(GL_TEXTURE_2D, *slot);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
    }
}

void Rendering::MaterialManager::LoadTexture(const char* filename, materialHandle_t material, TextureType_e type)
{

#ifdef _WIN32 
    const char* textures_dir = "res/textures/";
#else
    const char* textures_dir = "/res/textures/";
#endif

    // TODO: move to some tyoe definitions file or something
    // Using the more sensible linux max path length here, since 32k+ char limit in NTFS is quite insane
    char path[4096];
    const char* cwd = Configuration::Get().workingDirectory.c_str();
    snprintf(path, 4096, "%s%s%s", cwd, textures_dir, filename);

    int width, height, numComponents;
    unsigned char* data = stbi_load(path, &width, &height, &numComponents, 4);

    if (!data)
    {
        // TODO: Load some ugly generated default texture that doesn't require any files?
        printf("  !! ERROR: Unable to load texture: %s \n", filename);
        return;
    }

    this->Setup(material, type, width, height, data);

    stbi_image_free(data);
}

void Rendering::MaterialManager::SetUniforms(materialSlot_t resource)
{
    if (!this->boundShader || !this->boundShader->program) {
        return;
    }

    int useDiffuse = this->diffuse_maps[resource.slot] > 0;
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(this->boundShader->_uniforms.diffuse, 0);
    glUniform1i(this->boundShader->_uniforms.use_diffuse, useDiffuse);
    glBindTexture(GL_TEXTURE_2D, this->diffuse_maps[resource.slot]);

    int useSpecular = this->specular_maps[resource.slot] > 0;
    glActiveTexture(GL_TEXTURE0 + 1);
    glUniform1i(this->boundShader->_uniforms.specular, 1);
    glUniform1i(this->boundShader->_uniforms.use_specular, useSpecular);
    glBindTexture(GL_TEXTURE_2D, this->specular_maps[resource.slot]);

    int useNormal = this->normal_maps[resource.slot] > 0;
    glActiveTexture(GL_TEXTURE0 + 2);
    glUniform1i(this->boundShader->_uniforms.normal, 2);
    glUniform1i(this->boundShader->_uniforms.use_normal, useNormal);
    glBindTexture(GL_TEXTURE_2D, this->normal_maps[resource.slot]);

    int useAlpha = this->alpha_maps[resource.slot] > 0;
    glActiveTexture(GL_TEXTURE0 + 3);
    glUniform1i(this->boundShader->_uniforms.alpha, 3);
    glUniform1i(this->boundShader->_uniforms.use_alpha, useAlpha);
    glBindTexture(GL_TEXTURE_2D, this->alpha_maps[resource.slot]);
}

void Rendering::MaterialManager::Bind(materialHandle_t material, const Shader* shader)
{
    materialSlot_t resource = this->FindResource(material);
    this->boundShader = shader;
    SetUniforms(resource);
}

void Rendering::MaterialManager::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(this->boundShader->_uniforms.diffuse, 0);
    glUniform1i(this->boundShader->_uniforms.use_diffuse, 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glUniform1i(this->boundShader->_uniforms.specular, 0);
    glUniform1i(this->boundShader->_uniforms.use_specular, 0);

    glActiveTexture(GL_TEXTURE0 + 2);
    glUniform1i(this->boundShader->_uniforms.normal, 0);
    glUniform1i(this->boundShader->_uniforms.use_normal, 0);

    glActiveTexture(GL_TEXTURE0 + 3);
    glUniform1i(this->boundShader->_uniforms.alpha, 0);
    glUniform1i(this->boundShader->_uniforms.use_alpha, 0);

    this->boundShader = 0;
}

void Rendering::MaterialManager::Cleanup()
{
    for (int i = 0; i < this->_materials_top; i++) {
        glDeleteTextures(1, &this->diffuse_maps[i]);
        glDeleteTextures(1, &this->specular_maps[i]);
        glDeleteTextures(1, &this->normal_maps[i]);
        glDeleteTextures(1, &this->alpha_maps[i]);
    }

    this->diffuse_maps.Clear();
    this->specular_maps.Clear();
    this->normal_maps.Clear();
    this->alpha_maps.Clear();
}

materialSlot_t Rendering::MaterialManager::AllocateResource(materialHandle_t material)
{
    materialSlot_t resource;
    if (this->_materials_top < MAX_GAME_ENTITIES) {
        resource.slot = this->_materials_top;
        resource.material = material;
        this->_material_index[this->_materials_top] = resource;
        this->_materials_top += 1;
    }
    return resource;
}

materialSlot_t Rendering::MaterialManager::FindResource(materialHandle_t material)
{
    for (int i = 0; i < this->_materials_top; i++) {
        if (this->_material_index[i].material == material) {
            return this->_material_index[i];
        }
    }
    return materialSlot_t();
}

materialHandle_t Rendering::MaterialManager::FindMaterial(const char* name)
{
    for (int i = 0; i < this->_materials_top; i++) {
        int slot = this->_material_index[i].slot;
        if (this->names[slot] == name) {
            return this->_material_index[i].material;
        }
    }
    return materialHandle_t();
}
