#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "../lib/OpenGL.hpp"
#include "../EntityComponent.hpp"

class Texture : public EntityComponent {
public:
	Texture(const std::string &fileName);
	Texture();
	~Texture();

	virtual void Render(Shader& shader, Renderer& renderer, Camera& camera);

private:
	GLuint mTexture;
};

#endif