#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>
#include "GameObject.h"


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

	//void CubeVertex();
	//void CubeIndexes();

public:
	/*vector<uint> indices;
	vector<vec3> vertexs;*/	

	GameObject* root;
	vector<GameObject*> gameobjects;
};
