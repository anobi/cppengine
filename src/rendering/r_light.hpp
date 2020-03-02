#ifndef R_LIGHT_H
#define R_LIGHT_H

#include <glm/glm.hpp>
#include "../shader.hpp"
#include "../core/light.hpp"

namespace Rendering {
	class Light {
	public:
		virtual void UpdateShaderAttributes(std::shared_ptr<Shader> shader) {};
	private:
		std::shared_ptr<Light> _light;
		int _uniform_location = 0;
	};
}

#endif