#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>
#include "GameObject.h"
#include "ComponentCamera.h"
#include "MathGeoLib/Geometry/LineSegment.h"


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SaveScene();
	//void LoadScene(PHYSFS_File* sceneFile);

	void Draw();

	void AddCamera();

public:
	/*vector<uint> indices;
	vector<vec3> vertexs;*/	

	GameObject* root;

	vector<GameObject*> gameobjects;
	vector<ComponentCamera*> cameras;

	LineSegment ray;

	bool normals = false;
};
