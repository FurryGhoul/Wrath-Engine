#include "Application.h"
#include "Globals.h"
#include "ModuleInterface.h"
#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Glew/include/glew.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

	ImGui::SetNextWindowPos({ 0,20 });
	ImGui::SetNextWindowSize({ (float)App->window->width, (float)App->window->height });
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Docking", 0, window_flags);
	ImGui::PopStyleVar(3);

	//TODO: Create Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Configuration"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();

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
