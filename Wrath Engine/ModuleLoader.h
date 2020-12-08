#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include <list>

class Application;
using namespace std;

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
		
	bool CleanUp();
	bool Import(const string& pFile);

	bool ImportMesh(const aiMesh* mesh, ComponentMesh* compMesh, string path, aiColor3D colour);
	bool ImportMaterial(const aiMaterial* material, ComponentMaterial* compMat, string path);

	bool SaveMesh(ComponentMesh* compMesh);
	bool SaveMaterial(ComponentMaterial* compMat);

	bool LoadMesh(FILE* mesh);

	bool RecursiveLoadChildren(const aiScene* scene, const aiNode* node, GameObject* parent, string path);

	bool LoadGameObject();

	uint Texturing(ComponentMaterial* material, const char* file_name);

	//string module_name;
	string path;

	vec2 TextureSize;
};

