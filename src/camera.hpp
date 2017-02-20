#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "transform.hpp"

class Camera {
public:
	Camera() {
		mTransform = Transform();
		this->mProjection = glm::mat4();
	}

	Camera(glm::fmat4 projection){
		mTransform = Transform();
		this->mProjection = projection;
	}

	inline void SetAspectRatio(float fov, float aspectRatio) {
		this->mProjection = glm::perspective(
			fov, aspectRatio, 0.1f, 100.0f);
	}

	inline glm::fmat4 GetView() {
		return glm::lookAt(mTransform.GetPosition(),
			mTransform.GetPosition() + mTransform.GetDirection(),
			mTransform.GetUp());
	}

	inline glm::fmat4 GetProjection(){
		return mProjection;
	}

	inline glm::fmat4 GetViewProjection() {
		return mProjection * GetView();
	}

	inline glm::fvec3 GetPosition() { return mTransform.GetPosition(); }

	inline void LookAt(const glm::fvec3 &target) {
		mTransform.LookAt(target);
	}

	Transform mTransform;

private:
	glm::fmat4 mProjection;


};

#endif
