#pragma once
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/Geometry/Frustum.h"

class Module;
class Application;
class Plane;

class ComponentCamera : public Component
{
public:
	ComponentCamera(Component_Type type, GameObject* parent);
	~ComponentCamera();

	bool Update();

	void DrawFrustrum();
	void DrawRay();
	void CalculateAABB();

	bool ContainsAABB(const AABB& ref_box);
	void DrawCameraBoundingBox(const AABB& boundingbox);
	void SetAspectRatio(float aspect_ratio);

	float* GetViewMatrix();
	float* GetProjectionMatrix();

public:

	Frustum camera_frustum;
	AABB camera_BB;
};