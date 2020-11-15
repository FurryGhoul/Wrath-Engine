#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ComponentCamera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const vec &Spot);
	void Move(const vec &Movement);

	mat4x4 At(const vec3& right, const vec3& up, const vec3& direction);

public:
	ComponentCamera* editorCamera;
	
	vec X, Y, Z;
	mat4x4 Camera_view;

	vec cameraRef;
	vec cameraPos;
	vec3 cameraTarget;
	vec3 cameraFront;

	vec3 cameraDirection;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 newPos;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	Timer timer;
};