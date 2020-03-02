#ifndef R_GL_TEXTURE_H
#define R_GL_TEXTURE_H

#include <iostream>
#include "../render_entity.hpp"
#include "../../resources/res_texture.hpp"
#include "opengl.hpp"

namespace GL {
	class GLTexture  : Rendering::RenderEntity {

	public:
		GLTexture(std::shared_ptr<Resources::RTexture> texture);
		~GLTexture();

		void Draw(GLuint shader_program);
		void PostDraw(GLuint shader_program);

	private:
		std::string _type = "";
		unsigned int _i = 0;  // Not sure what this is but oh well. Somekind of a layout/order thing.
		GLuint _id = 0;
	};
}

#endif