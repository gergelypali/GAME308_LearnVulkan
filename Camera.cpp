#include "Camera.h"
#include "MMath.h"


Camera::Camera() {
	projection.loadIdentity();/// it did it autamatically; but this just reminds me
	view.loadIdentity();
}

Camera::~Camera() {
	/// There's nothing to do here yet
}

void Camera::UpdateViewMatrix()
{
	view = MMath::lookAt(eyePos, staringAt, cameraUp);
}

void Camera::UpdateProjectionMatrix(const float& aspectRatio)
{
	projection = MMath::perspective(45.0f, aspectRatio, 0.5f, 200.0f);
}
