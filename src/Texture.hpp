#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "lib/OpenGL.hpp"
#include "EntityComponent.hpp"

class Texture : public EntityComponent {
public:
	Texture(const std::string &fileName);
	Texture();
	~Texture();

	void Render(Renderer& renderer){
        glBindTexture(GL_TEXTURE_2D, mTexture);
    }

private:
	GLuint mTexture;
};

#endif
