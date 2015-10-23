#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

#include "lib/Vector.hpp"
#include "lib/OpenGL.hpp"


typedef struct {
    vector3<float> normal;
    vector3<float> position;
    vector3<float> color;
} vertex_t;

typedef struct {
    std::vector<vertex_t> vertices;
    GLuint shader;
    // + textures, shaders etc..
} renderEntity_t;

/////////////////
// E N T I T Y //
/////////////////

class Entity {

private:
    vector3<float> location;
    bool alive;
    GLuint LoadShader(const char* vertex_shader_path, const char* fragment_shader_path);
    std::string ReadFile(const char* filePath);

public:
    Entity();
    ~Entity();

    renderEntity_t renderEntity;

	void Init();
    void Spawn();
	void Update();
    void Kill();
    void Remove();
    vector3<float> GetLocation();
    void SetLocation(float x, float y, float z);
	void MakeTestEntity();
};

#endif
