#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

//void ModuleSceneIntro::CubeIndexes()
//{
//	GLuint ibo, vbo = 0;
//
//	indices = { 0, 1, 2, 0, 3, 2,			//FRONT
//					 3, 0, 4, 5, 3, 4,			//RIGHT
//					 6, 2, 1, 6, 7, 2,			//LEFT
//					 5, 4, 6, 6, 4, 7,			//BACK
//					 3, 6, 1, 6, 3, 5,			//TOP
//					 0, 2, 7, 7, 4, 0 };		//BOT
//
//	vertexs.push_back((1.0f, 0.0f, 0.0f));
//	vertexs.push_back((0.0f, 1.0f, 0.0f));
//	vertexs.push_back((0.0f, 0.0f, 0.0f));
//	vertexs.push_back((1.0f, 1.0f, 0.0f));
//
//	vertexs.push_back((1.0f, 0.0f, -1.0f));
//	vertexs.push_back((1.0f, 1.0f, -1.0f));
//	vertexs.push_back((0.0f, 1.0f, -1.0f));
//	vertexs.push_back((0.0f, 0.0f, -1.0f));
//
//	glGenBuffers(1, (GLuint*)&(ibo));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices, GL_STATIC_DRAW);
//
//	glGenBuffers(1, (GLuint*)&(vbo));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexs.size(), &vertexs, GL_STATIC_DRAW);
//}