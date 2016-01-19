#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "lib/OpenGL.hpp"

class Texture {
public:
	Texture(const std::string &fileName);
	Texture();
	~Texture();
	void Bind();

private:
	GLuint mTexture;
};

#endif