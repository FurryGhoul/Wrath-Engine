#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	float* GetViewMatrix();
	//float* GetProjectionMatrix();

	mat4x4 At(const vec3& right, const vec3& up, const vec3& direction);

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z;
	mat4x4 Camera_view;

	vec3 cameraRef;
	vec3 cameraPos;
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