#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "DevIL/includes/il.h"
#include "DevIL/includes/ilu.h"
#include "DevIL/includes/ilut.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment( lib, "DevIL/libx86/DevIL.lib")
#pragma comment( lib, "DevIL/libx86/ILU.lib")
#pragma comment( lib, "DevIL/libx86/ILUT.lib")

typedef unsigned int uint;
using namespace Assimp;

ModuleLoader::ModuleLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	glEnable(GL_TEXTURE_2D);
}

ModuleLoader::~ModuleLoader() {}

bool ModuleLoader::Init(const JSON_Object& config) 
{ 
	return true; 
}

bool ModuleLoader::Start() 
{ 
	return true; 
}

update_status ModuleLoader::PreUpdate(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

update_status ModuleLoader::Update(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

update_status ModuleLoader::PostUpdate(float dt) 
{ 
	return UPDATE_CONTINUE; 
}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}