#ifndef SCENE0_H
#define SCENE0_H
#include <memory>
#include "Scene.h"
#include "Vector.h"
#include "Renderer.h"
#include "Camera.h"
#include "Quaternion.h"
#include "QMath.h"
#include "PointLight.h"
#include "Model.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene0 : public Scene {
private:
	Renderer* renderer;
	Camera* camera;
	Matrix4 mariosModelMatrix;
	Matrix4 skull1ModelMatrix;
	Matrix4 skull2ModelMatrix;
	Matrix4 floorModelMatrix;
	Vec4 lightPos[3];
	Vec4 lightColor[3] = {Vec4{0,255,0,0}, Vec4{255,0,0,0}, Vec4{0,0,255,0}};

	Quaternion spinnerQ{};
	Vec3 spinnerAxis{};
	std::unique_ptr<PointLight> light1;
	std::unique_ptr<PointLight> light2;
	std::unique_ptr<PointLight> light3;
	std::unique_ptr<Model> mario;
	std::unique_ptr<Model> skull1;
	std::unique_ptr<Model> skull2;
	std::unique_ptr<Model> floor;

public:
	explicit Scene0(Renderer* renderer_);
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H