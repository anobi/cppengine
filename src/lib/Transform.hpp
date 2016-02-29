#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform {
public:
	Transform() {
		this->position = glm::fvec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::fvec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::fvec3(1.0f, 1.0f, 1.0f);
	}

	glm::fmat4 GetModel() {
		glm::fmat4 positionMatrix	= glm::translate(position);
		glm::fmat4 scaleMatrix		= glm::scale(scale);
		glm::fmat4 rotationX		= glm::rotate(rotation.x, glm::fvec3(1.0, 0.0, 0.0));
		glm::fmat4 rotationY		= glm::rotate(rotation.y, glm::fvec3(0.0, 1.0, 0.0));
		glm::fmat4 rotationZ		= glm::rotate(rotation.z, glm::fvec3(0.0, 0.0, 1.0));
		glm::fmat4 rotationMatrix	= rotationX * rotationY * rotationZ;

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	glm::fmat4 GetModelViewProjection(const glm::fmat4 &viewProjection) {
		glm::fmat4 model = GetModel();
		return viewProjection * model;
	}

	inline glm::fvec3& GetPosition() { return this->position; }
	inline glm::fvec3& GetRotation() { return this->rotation; }
	inline glm::fvec3& GetScale() { return this->scale; }

	inline void SetPosition(const glm::fvec3 &position) { this->position = position; }
	inline void SetRotation(const glm::fvec3 &rotation) { this->rotation = rotation; }
	inline void SetScale(const glm::fvec3 &scale) { this->scale = scale; }

private:
	glm::fvec3 position;
	glm::fvec3 rotation;
	glm::fvec3 scale;
};

#endif
