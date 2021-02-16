#ifndef R_GL_LIGHT
#define R_GL_LIGHT

#include <glm/glm.hpp>
#include "../../core/light.hpp"
#include "../r_light.hpp"

namespace Rendering {
	namespace GL {
		class GLPointLight : public Rendering::Light {
		public:
			GLPointLight(std::shared_ptr<Core::PointLight> light) { this->_light = light; };
			~GLPointLight() {}

			void UpdateShaderAttributes(std::shared_ptr<Shader> shader);

		protected:
			std::shared_ptr<Core::PointLight> _light;
			int uniform_location = 32;
		};

		class GLDirectionalLight : public Rendering::Light {
		public:
			GLDirectionalLight(std::shared_ptr<Core::DirectionalLight> light) { this->_light = light; };
			~GLDirectionalLight() {}

			void UpdateShaderAttributes(std::shared_ptr<Shader> shader);

		protected:
			std::shared_ptr<Core::DirectionalLight> _light;
			int uniform_location = 64;
		};
	}
}
#endif