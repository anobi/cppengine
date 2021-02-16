#ifndef R_GL_TEXTURE
#define R_GL_TEXTURE

#include <iostream>
#include "../render_entity.hpp"
#include "../../resources/res_texture.hpp"
#include "opengl.hpp"

namespace Rendering {
	namespace GL {
		class GLTexture : Rendering::RenderEntity {

		public:
			GLTexture(std::shared_ptr<Resources::RTexture> texture);
			~GLTexture();

			void Draw(GLuint shader_program);
			void PostDraw(GLuint shader_program);

		private:
			std::string _type = "";
			unsigned int _texture_index = 0;
			GLuint _id = 0;
		};
	}
}

#endif