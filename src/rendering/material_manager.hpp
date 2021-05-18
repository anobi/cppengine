#ifndef __RENDERING__MATERIAL_MANAGER_H__
#define __RENDERING__MATERIAL_MANAGER_H__

#include <string>

#include "../opengl.hpp"
#include "../shader.hpp"

#include "../constants.hpp"
#include "../containers/array.hpp"
#include "../entities/entity_handle.hpp"

constexpr int MAX_MATERIALS = 1024;

struct materialHandle_t
{
    unsigned int id = INVALID_HANDLE_ID;

    inline bool valid()
    {
        return (id != INVALID_HANDLE_ID);
    };

    inline bool operator==(materialHandle_t other) 
    {
        return this->id == other.id;
    }

    inline bool operator!=(materialHandle_t other)
    {
        return this->id != other.id;
    }
};

struct materialSlot_t 
{
    materialHandle_t material;
    unsigned int slot;

    inline bool valid() 
    {
        return (material.valid() && slot != INVALID_SLOT);
    };
};

enum TextureType_e {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    ALPHA
};

namespace Rendering 
{
    class MaterialManager
    {
    public:
        MaterialManager() {};
        ~MaterialManager() {};

        materialHandle_t Add(const char* name);
        materialHandle_t FindMaterial(const char* name);

        void Setup(materialHandle_t material, TextureType_e type, const int width, const int height, const void* data);
        void LoadTexture(const char* filename, materialHandle_t material, TextureType_e type);
        void SetUniforms(materialSlot_t resource);
        void Bind(materialHandle_t material, const Shader* shader);
        void Unbind();
        void Cleanup();

        unsigned int _next_id = 1;
        unsigned int _materials_top = 0;
        Array<materialSlot_t, MAX_GAME_ENTITIES> _material_index;
        materialSlot_t AllocateResource(materialHandle_t material);
        materialSlot_t FindResource(materialHandle_t material);

        Array<std::string, MAX_MATERIALS> names;
        Array<GLuint, MAX_MATERIALS> diffuse_maps;
        Array<GLuint, MAX_MATERIALS> specular_maps;
        Array<GLuint, MAX_MATERIALS> normal_maps;
        Array<GLuint, MAX_MATERIALS> alpha_maps;

        const Shader* boundShader = nullptr;
    };
}


#endif // __RENDERING__RENDER_MATERIAL_H__