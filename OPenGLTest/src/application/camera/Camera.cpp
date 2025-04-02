#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera()
{
}
glm::mat4 Camera::getViewMatrix()
{
	glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + front;
	glm::mat4 viewMatrix = glm::lookAt(mPosition, center, mUp);

	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::identity<glm::mat4>();
}

void Camera::scale(float deltaScale)
{
}
