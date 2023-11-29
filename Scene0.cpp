#include <iostream>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "VulkanRenderer.h"
#include "Camera.h"

Scene0::Scene0(Renderer *renderer_): 
	Scene(nullptr),renderer(renderer_), camera(nullptr) {
	camera = new Camera();
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	if(camera) delete camera;

}

bool Scene0::OnCreate() {
	int width = 0, height = 0;
	float aspectRatio;
	switch (renderer->getRendererType()){
	case RendererType::VULKAN:
		
		SDL_GetWindowSize(dynamic_cast<VulkanRenderer*>(renderer)->GetWindow(), &width, &height);
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		camera->UpdateProjectionMatrix(aspectRatio);
		camera->UpdateViewMatrix();

		break;
	}

	return true;
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_WINDOWEVENT:
		if (sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			printf("size changed %d %d\n", sdlEvent.window.data1, sdlEvent.window.data2);
			float aspectRatio = static_cast<float>(sdlEvent.window.data1) / static_cast<float>(sdlEvent.window.data2);
			camera->UpdateProjectionMatrix(aspectRatio);
		}
		break;
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			if(MATH::VMath::mag(camera->GetEyePos()) > 4.f)
			{
				camera->SetEyePos(camera->GetEyePos() - MATH::VMath::normalize(camera->GetEyePos()));
			}
			break;
		case SDL_SCANCODE_S:
			camera->SetEyePos(camera->GetEyePos() + MATH::VMath::normalize(camera->GetEyePos()));
			break;
		case SDL_SCANCODE_UP:
			spinnerQ = MATH::QMath::angleAxisRotation(-5.f, MATH::VMath::cross(camera->GetCameraUp(), camera->GetEyePos()));
			camera->SetEyePos(MATH::QMath::rotate( camera->GetEyePos(), spinnerQ ));
			camera->SetCameraUp(MATH::QMath::rotate(camera->GetCameraUp(), spinnerQ));
			break;
		case SDL_SCANCODE_DOWN:
			spinnerQ = MATH::QMath::angleAxisRotation(5.f, MATH::VMath::cross(camera->GetCameraUp(), camera->GetEyePos()));
			camera->SetEyePos(MATH::QMath::rotate(camera->GetEyePos(), spinnerQ));
			camera->SetCameraUp(MATH::QMath::rotate(camera->GetCameraUp(), spinnerQ));
			break;
		case SDL_SCANCODE_LEFT:
			spinnerQ = MATH::QMath::angleAxisRotation(5.f, camera->GetCameraUp());
			camera->SetEyePos(MATH::QMath::rotate(camera->GetEyePos(), spinnerQ));
			break;
		case SDL_SCANCODE_RIGHT:
			spinnerQ = MATH::QMath::angleAxisRotation(-5.f, camera->GetCameraUp());
			camera->SetEyePos(MATH::QMath::rotate(camera->GetEyePos(), spinnerQ));
			break;
		}
	}
}
void Scene0::Update(const float deltaTime) {
	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime;
	float rad = elapsedTime / 1000.f;
	mariosModelMatrix = MMath::rotate(180.f, Vec3(0.0f, 1.0f, 0.0f));
	lightPos.x = 2.f * sinf(rad);
	lightPos.y = 2.f * cosf(rad);
	std::cout << "ASD" << std::endl;
	std::cout << lightPos.x << " ";
	std::cout << lightPos.y << " ";
	std::cout << "ASD" << std::endl;
}

void Scene0::Render() const {
	
	switch (renderer->getRendererType()) {

	case RendererType::VULKAN:
		VulkanRenderer* vRenderer;
		vRenderer = dynamic_cast<VulkanRenderer*>(renderer);
		camera->UpdateViewMatrix();
		vRenderer->SetCameraUBO(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		vRenderer->SetGlobalLightingUBO(lightPos, Vec4{ 0.1f, 0.1f, 0.1f, 0 });
		vRenderer->SetModelUBO(mariosModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(mariosModelMatrix)));
		vRenderer->Render();
		break;
	}
}


void Scene0::OnDestroy() {
	
}
