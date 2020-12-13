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

	void SaveScene(string fileName);
	void LoadScene(string filePath);

	void Draw();
	void GameObjectsToDraw();

	void DeleteGameObject(GameObject* objecttodelete, bool erasefromparent = true);

	void AddCamera();

public:
	/*vector<uint> indices;
	vector<vec3> vertexs;*/	

	GameObject* root;

	vector<GameObject*> gameobjects;
	vector<GameObject*> objectsToDraw;
	vector<ComponentCamera*> cameras;

	LineSegment ray;

	bool normals = false;
};
