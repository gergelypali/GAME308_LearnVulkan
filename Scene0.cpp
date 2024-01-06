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

	printf("sceneOnCreate1\n");
	light1 = std::make_unique<PointLight>(dynamic_cast<VulkanRenderer*>(renderer));
	light2 = std::make_unique<PointLight>(dynamic_cast<VulkanRenderer*>(renderer));
	light3 = std::make_unique<PointLight>(dynamic_cast<VulkanRenderer*>(renderer));
	printf("sceneOnCreate2\n");
	light1->setColor(lightColor[0]);
	light1->OnCreate();
	light2->setColor(lightColor[1]);
	light2->OnCreate();
	light3->setColor(lightColor[2]);
	light3->OnCreate();
	printf("sceneOnCreate3\n");
	mario = std::make_unique<Model>(dynamic_cast<VulkanRenderer*>(renderer), "./meshes/Mario.obj", "textures/mario_fire.png");
	printf("sceneOnCreate4\n");
	mario->OnCreate();
	printf("sceneOnCreate5\n");
	skull1 = std::make_unique<Model>(dynamic_cast<VulkanRenderer*>(renderer), "./meshes/Skull.obj", "textures/mario_fire.png");
	printf("sceneOnCreate6\n");
	skull1->OnCreate();
	printf("sceneOnCreate7\n");
	skull2 = std::make_unique<Model>(dynamic_cast<VulkanRenderer*>(renderer), "./meshes/Skull.obj", "textures/mario_fire.png");
	printf("sceneOnCreate8\n");
	skull2->OnCreate();
	floor = std::make_unique<Model>(dynamic_cast<VulkanRenderer*>(renderer), "./meshes/quad.obj", "textures/grassTexture.png");
	printf("sceneOnCreate8\n");
	floor->OnCreate();
	printf("sceneOnCreate9\n");
	dynamic_cast<VulkanRenderer*>(renderer)->createCommandBuffers();
	printf("sceneOnCreate10\n");

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
	skull1ModelMatrix  = MMath::translate(Vec3(1,1,0)) * MMath::scale(Vec3(0.2,0.2,0.2));
	skull2ModelMatrix  = MMath::translate(Vec3(-1,1,0)) * MMath::scale(Vec3(0.2,0.2,0.2));
	floorModelMatrix  = MMath::translate(Vec3(0,-2.5,0)) * MMath::rotate(180.f, Vec3(1.0f, 0.0f, 0.0f)) *  MMath::scale(Vec3(5,5,5));
	lightPos[0].x = 2.f * sinf(rad);
	lightPos[0].y = 2.f * cosf(rad);
	lightPos[1].x = 2.f * sinf(rad + 2.05);
	lightPos[1].y = 2.f * cosf(rad + 2.05);
	lightPos[2].x = 2.f * sinf(rad + 4.1);
	lightPos[2].y = 2.f * cosf(rad + 4.1);

	//mariosModelMatrix.print();
	//camera->GetViewMatrix().print();
	//camera->GetProjectionMatrix().print();
}

void Scene0::Render() const {
	
	switch (renderer->getRendererType()) {

	case RendererType::VULKAN:
		VulkanRenderer* vRenderer;
		vRenderer = dynamic_cast<VulkanRenderer*>(renderer);
		camera->UpdateViewMatrix();
		//camera->GetViewMatrix().print();
		//camera->GetProjectionMatrix().print();
		light1->Update(lightPos[0], camera->GetViewMatrix(), camera->GetProjectionMatrix());
		light2->Update(lightPos[1], camera->GetViewMatrix(), camera->GetProjectionMatrix());
		light3->Update(lightPos[2], camera->GetViewMatrix(), camera->GetProjectionMatrix());
		//vRenderer->SetCameraUBO(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		//vRenderer->SetGlobalLightingUBO(lightPos, Vec4{ 0.1f, 0.1f, 0.1f, 0 });
		//vRenderer->SetModelUBO(mariosModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(mariosModelMatrix)));
		mario->Update(mariosModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(mariosModelMatrix)), camera->GetViewMatrix(), camera->GetProjectionMatrix(), lightPos, lightColor);
		skull1->Update(skull1ModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(skull1ModelMatrix)), camera->GetViewMatrix(), camera->GetProjectionMatrix(), lightPos, lightColor);
		skull2->Update(skull2ModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(skull2ModelMatrix)), camera->GetViewMatrix(), camera->GetProjectionMatrix(), lightPos, lightColor);
		floor->Update(floorModelMatrix, MATH::MMath::inverse(MATH::MMath::transpose(floorModelMatrix)), camera->GetViewMatrix(), camera->GetProjectionMatrix(), lightPos, lightColor);
		vRenderer->Render();
		break;
	}
}

void Scene0::OnDestroy() {
	printf("scene0_OnDestroy1\n");
	//light1->OnDestroy();
	printf("scene0_OnDestroy2\n");
}



/*

	//vec4 ks = glights.diffuse;
	//vec4 kd = glights.diffuse;
	vec4 ks = vec4(0.1, 0.1, 0.1, 0);
	vec4 kd = vec4(0.1, 0.1, 0.1, 0);
	vec4 kt = texture(texSampler,fragTexCoords);
	vec4 ka = 0.01 * kt;
	
	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based incedent beam of light which means a vector 
	/// from the light source not the direction to the light source. 
	/// Put a minus sign on light dir to turn it in the opposite direction. 
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));

	float spec = max(dot(eyeDir, reflection), 0.0);
	spec = pow(spec,14.0);
	
	fragColor = ka;
	fragColor +=  diff * (kt + kd) + spec * ks;
	//fragColor *= vec4(1,0,1,0);
	fragColor = kt;






	vec3 vertexWorldPos = mat3(bigHuge.model) * vVertex.xyz;

	fragTexCoords = texCoords;
	mat4 modelM = mat4(
				-1.0, 0.0, 0.0, 0.0,  // 1. column
				0.0, 1.0, 0.0, 0.0,  // 2. column
				0.0, 0.0, -1.0, 0.0,  // 3. column
				0.0, 0.0, 0.0, 1.0); // 4. column
	mat4 view = mat4(
				1.0, 0.0, 0.0, 0.0,  // 1. column
				0.0, 1.0, 0.0, 0.0,  // 2. column
				0.0, 0.0, 1.0, 0.0,  // 3. column
				0.0, 0.0, -5.0, 1.0); // 4. column
	mat4 proj = mat4(
				1.3, 0.0, 0.0, 0.0,  // 1. column
				0.0, 2.4, 0.0, 0.0,  // 2. column
				0.0, 0.0, -1.0, -1.0,  // 3. column
				0.0, 0.0, -1.0, 0.0); // 4. column
	vertNormal = normalize(mat3(bigHuge.normal) * vNormal.xyz); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(bigHuge.viewMatrix * bigHuge.model) * vVertex.xyz; /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	lightDir = normalize(bigHuge.position.xyz - vertexWorldPos.xyz); /// Create the light direction.
	
	gl_Position =  bigHuge.projectionMatrix * bigHuge.viewMatrix * bigHuge.model * vec4(vVertex, 1.f);
	//gl_Position =  proj * view * modelM * vec4(vVertex, 1.f);
	gl_Position.y *= -1.f;
*/