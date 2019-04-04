#ifndef R_GL_TEXTURE_H
#define R_GL_TEXTURE_H

#include <iostream>
#include "../renderable.hpp"
#include "../../resources/texture.hpp"

namespace Rendering {
	class Texture  : Renderable {

	public:
		Texture(std::shared_ptr<Resources::Texture> texture);
		~Texture();
		void Draw(GLuint shader_program);
		void PostDraw(GLuint shader_program);

	private:
		std::string _type = "";
		unsigned int _i = 0;  // Not sure what this is but oh well. Somekind of a layout/order thing.
		GLuint _id = 0;
	};
}

#endif