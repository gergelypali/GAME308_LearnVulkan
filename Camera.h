#pragma once
#include "Matrix.h"
using namespace MATH;
class Camera {
private:
	Matrix4 projection;
	Matrix4 view;
	Matrix4 rotation;
	Matrix4 translate;

	Vec3 cameraUp{ 0, 1.f, 0 };
	Vec3 eyePos{ 0.0f, 0.0f, 5.0f };
	Vec3 staringAt{ 0.0f, 0.0f, 0.0f };

public:
	Camera();
	~Camera();
	//inline void SetProjectionMatrix(const Matrix4& newProj) { projection = newProj; }
	//inline void SetViewMatrix(const Matrix4& newView) { view = newView; }
	inline Matrix4 GetProjectionMatrix() { return projection; }
	inline Matrix4 GetViewMatrix() { return view; }

	inline void SetCameraUp(const Vec3& newUp) { cameraUp = newUp; }
	inline Vec3 GetCameraUp() { return cameraUp; }
	inline void SetEyePos(const Vec3& pos) { eyePos = pos; }
	inline Vec3 GetEyePos() { return eyePos; }
	inline void SetStaringAt(const Vec3& staring) { staringAt = staring; }
	inline Vec3 GetStaringAt() { return staringAt; }

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(const float& aspectRatio);
};

