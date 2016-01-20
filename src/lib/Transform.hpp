#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.hpp"

struct Transform {
public:
	Transform() {
		this->position = glm::vec3();
		this->rotation = glm::vec3();
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	inline glm::mat4 GetModel() const {
		glm::mat4 positionMatrix	= glm::translate(position);
		glm::mat4 scaleMatrix		= glm::scale(scale);
		glm::mat4 rotationX			= glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotationY			= glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZ			= glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotationMatrix	= rotationX * rotationY * rotationZ;

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	inline glm::mat4 GetModelViewProjection(const Camera& camera) const {
		glm::mat4 viewProjection = camera.GetViewProjection();
		glm::mat4 model = GetModel();

		return viewProjection * model;
	}

	inline glm::vec3 *Position() { return &this->position; }
	inline glm::vec3 *Rotation() { return &this->rotation; }
	inline glm::vec3 *Scale() { return &this->scale; }

	inline void SetPosition(const glm::vec3 &position) { this->position = position; }
	inline void SetRotation(const glm::vec3 &rotation) { this->rotation = rotation; }
	inline void SetScale(const glm::vec3 &scale) { this->scale = scale; }

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

#endif
