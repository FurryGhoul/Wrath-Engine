#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "ImGui/imgui_impl_opengl2.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	glewInit();
	GLenum err = glewInit();

	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

update_status ModuleRenderer3D::Update(float dt)
{
	for (int i = 0; i < App->scene->mainGOs.size(); ++i)
	{
		App->scene->mainGOs[i]->CalculateGlobal();
	}

	if (lightning) { lights[0].Active(true); }
	else lights[0].Active(false);
	if (backface) { glEnable(GL_CULL_FACE); }
	else { glDisable(GL_CULL_FACE); }
	if (!lights_on) { glEnable(GL_LIGHTING); }
	else { glDisable(GL_LIGHTING); }
	if (textured) { glEnable(GL_TEXTURE_2D); }
	else { glDisable(GL_TEXTURE_2D); }

	for (int i = 0; i < App->scene->gameobjects.size(); ++i)
	{
		if (App->scene->gameobjects[i]->selected)
		{
			DrawBoundingBox(App->scene->gameobjects[i]->boundingBox);
		}

		if (boundingBoxes)
		{
			DrawBoundingBox(App->scene->gameobjects[i]->boundingBox);
		}
	}

	glEnable(GL_COLOR_MATERIAL);

	return UPDATE_CONTINUE;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, buffer_tex);

	glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO: update this shit when frustrum is done
	//glMatrixMode(App->camera->editorCamera->GetProjectionMatrix());
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->editorCamera->GetViewMatrix());

	lights[0].SetPos(App->camera->cameraPos.x, App->camera->cameraPos.y, App->camera->cameraPos.z);
	for (uint i = 0; i < MAX_LIGHTS; ++i) lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	int result = 0;
	App->scene->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::Render();
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	glDeleteFramebuffers(1, &fbo);
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::ActivateWireframe()
{
	if (wireframe) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
}

void  ModuleRenderer3D::DrawNormals(ComponentMesh* mesh)
{
	glLineWidth(2.0f);
	glColor3f(1, 0, 0);

	glBegin(GL_LINES);

	for (uint i = 0; i < mesh->num_vertices * 3; i += 3)
	{
		glVertex3f(mesh->vertices[i] - mesh->normals[i], mesh->vertices[i + 1] - mesh->normals[i + 1], mesh->vertices[i + 2] - mesh->normals[i + 2]);
		glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
	}
	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1);
}

void ModuleRenderer3D::DrawBoundingBox(const AABB& boundingbox)
{
	glLineWidth(2.f);
	glColor3f(0.f, 1.f, 0.f);

	glBegin(GL_LINES);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	App->window->width = width;
	App->window->height = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (fbo >= 0)
	{
		glDeleteFramebuffers(1, &fbo);
	}

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &buffer_tex);
	glBindTexture(GL_TEXTURE_2D, buffer_tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer_tex, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
