#include "Application.h"
#include "Globals.h"
#include "ModuleInterface.h"
#include "ModuleWindow.h"

#include "MathGeoLib/MathGeoLib.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl2.h"

ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleInterface::~ModuleInterface() {}

bool ModuleInterface::Start()
{
	LOG("Loading UI");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	return ret;
}

bool ModuleInterface::CleanUp()
{
	LOG("Unloading UI");

	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_GL_DeleteContext(App->window->window);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

update_status ModuleInterface::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleInterface::Update(float dt)
{
	ImGui::ShowDemoWindow();

	return UPDATE_CONTINUE;
}

update_status ModuleInterface::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
