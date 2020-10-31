#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include <list>

class Application;
using namespace std;

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(const JSON_Object& config);
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
		
	bool CleanUp();
	bool Import(const string& pFile);

	uint Texturing(ComponentMaterial* material, const char* file_name);

	//string module_name;
	string path;

	vec2 TextureSize;
};

