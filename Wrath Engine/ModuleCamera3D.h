#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ComponentCamera.h"

struct Hit_Object
{
	Hit_Object(GameObject* gameobject, float distance)
	{
		gameObject = gameobject;
		this->distance = distance; 
	}
	GameObject* gameObject = nullptr;
	float distance = 0;
};

struct Hit_Distance
{
	bool operator()(const Hit_Object* object1, const Hit_Object* object2) const
	{
		bool ret = false;
		if (object1->distance > object2->distance)
		{
			ret = true;
		}

		return ret;
	}
};

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

	GameObject* StartMousePicking();

	float FindClosest(GameObject* hitobject, LineSegment ray);

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