#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
public:
	Camera(glm::vec3 position, float fov, float aspectRatio, float zNear, float zFar) {
		this->position = position;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspectRatio, zNear, zFar);
	}

	glm::mat4 GetViewProjection() const {
		return projection * glm::lookAt(position, position + forward, up);
	}

private:
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
};

#endif
