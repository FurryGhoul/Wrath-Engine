#include "Application.h"
#include "Globals.h"
#include "ModuleInterface.h"
#include "ModuleWindow.h"

#include "UI_Config.h"
#include "UI_Scene.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Glew/include/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl2.h"

#include "Devil/includes/il.h"
#include "Assimp/include/version.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.WantCaptureKeyboard = true;
	io.WantCaptureMouse = true;
	io.WantTextInput = true;

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

	bool exit = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Scene", false, true);

			if (ImGui::MenuItem("Save Scene", NULL, &save_scene_open))
			{
				//App->scene->SaveScene();
			}

			if (ImGui::MenuItem("Close", "ALT+F4"))
			{
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				ImGui::End();
				ImGui::EndFrame();
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, &about_open)) {}
			if (ImGui::MenuItem("Hardware Info", NULL, &hardware_open)) {}
			if (ImGui::MenuItem("GitHub", false, true)) { App->RequestBrowser("https://github.com/FurryGhoul/Wrath-Engine"); }
			if (ImGui::MenuItem("Controls", NULL, &controls_open)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Configuration", NULL, &config_open)) {}
			if (ImGui::MenuItem("Hierarchy", NULL, &hierarchy_open)) {}
			if (ImGui::MenuItem("Inspector", NULL, &inspector_open)) {}
			if (ImGui::MenuItem("Console", NULL, &console_open)) {}
			if (ImGui::MenuItem("Scene", NULL, &scene_open)) {}
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
	if (about_open)					this->CreateAboutWindow(&about_open);
	if (hardware_open)				this->CreateHardwareWindow(&hardware_open);
	if (controls_open)				this->CreateControlsWindow(&controls_open);
	if (config_open)				App->configuration->Draw(&config_open);
	if (scene_open)					App->scene_ui->Draw(&scene_open);
	if (hierarchy_open)				App->hierarchy->Draw(&hierarchy_open);
	if (console_open)				App->console->Draw(&console_open);
	if (inspector_open)				App->inspector->Draw(App->hierarchy->selectedGO, &inspector_open);
	if (save_scene_open)			this->CreateSaveSceneWindow(&save_scene_open);

	//ImGui::ShowDemoWindow();

	return UPDATE_CONTINUE;
}

update_status ModuleInterface::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleInterface::CreateAboutWindow(bool* open)
{
	ImGui::OpenPopup("About");
	if (ImGui::BeginPopupModal("About"))
	{
		ImGui::Text("Wrath Engine"), ImGui::Separator();
		ImGui::Text("This is a video game engine with academic purposes.");
		ImGui::Text("By Ivan Drofiak.");
		ImGui::Text("Licensed under the MIT License."), ImGui::Separator();

		ImGui::Text("External Libraries We Used:");
		if (ImGui::Button("MathGeoLib")) App->RequestBrowser("http://clb.demon.fi/MathGeoLib/"); ImGui::SameLine();
		if (ImGui::Button("SDL")) App->RequestBrowser("https://wiki.libsdl.org/FrontPage"); ImGui::SameLine();
		if (ImGui::Button("Open GL")) App->RequestBrowser("https://www.opengl.org/"); ImGui::SameLine();
		if (ImGui::Button("Parson")) App->RequestBrowser("https://github.com/kgabis/parson"); ImGui::SameLine();
		if (ImGui::Button("DevIL")) App->RequestBrowser("http://openil.sourceforge.net/"); ImGui::SameLine();
		if (ImGui::Button("Glew")) App->RequestBrowser("http://glew.sourceforge.net/"); ImGui::SameLine();
		if (ImGui::Button("Assimp")) App->RequestBrowser("http://www.assimp.org/"); 	ImGui::Separator();
		if (ImGui::Button("ParShapes")) App->RequestBrowser("https://github.com/prideout/par"); 	ImGui::Separator();
		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); about_open = false;}
		ImGui::EndPopup();
	}
}

void ModuleInterface::CreateHardwareWindow(bool* open)
{
	ImGui::OpenPopup("Hardware Info");
	if (ImGui::BeginPopupModal("Hardware Info"))
	{
		static bool active = false;
		ImGui::Checkbox("Active", &active);

		//Versions
		float mathgeolibversion = 2.0f;
		SDL_version ver;
		SDL_GetVersion(&ver);


		ImGui::Text("SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);
		ImGui::Text("Assimp Version: %i", aiGetVersionMajor());
		ImGui::Text("Open GL Version: %s", glGetString(GL_VERSION));
		ImGui::Text("DevIL Version: %i", IL_VERSION);
		ImGui::Text("MathGeoLib Version: %.02f", mathgeolibversion);
		ImGui::Text("Glew Version: %s", glewGetString(GLEW_VERSION));

		ImGui::Separator();

		//CPU Cach�
		int cpu_cache = SDL_GetCPUCount();
		ImGui::Text("CPU's: %i", cpu_cache);

		//RAM 
		float ram = SDL_GetSystemRAM();
		ImGui::Text("System RAM: %f Gb", ram / 1000);

		//Caps
		SDL_bool SDL_HasRDTSC();	 if (SDL_HasRDTSC) { RDTSC = "RDTSC"; }
		SDL_bool SDL_HasMMX();		 if (SDL_HasMMX) { MMX = "MMX"; }
		SDL_bool SDL_HasAVX();		 if (SDL_HasAVX) { AVX = "AVX"; }
		SDL_bool SDL_HasSSE();	     if (SDL_HasSSE) { SSE = "SSE"; }
		SDL_bool SDL_HasSSE2();		 if (SDL_HasSSE2) { SSE2 = "SSE2"; }
		SDL_bool SDL_HasSSE3();		 if (SDL_HasSSE3) { SSE3 = "SSE3"; }
		SDL_bool SDL_HasSSE41();	 if (SDL_HasSSE41) { SSE41 = "SSE41"; }
		SDL_bool SDL_HasSSE42();	 if (SDL_HasSSE42) { SSE42 = "SSE42"; }

		ImGui::Text("Caps: %s, %s, %s, %s, %s, %s, %s, %s", RDTSC, MMX, AVX, SSE, SSE2, SSE3, SSE41, SSE42);

		ImGui::Separator();
		ImGui::Text("GPU Vendor: "); ImGui::SameLine(); ImGui::Text((char*)glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: "); ImGui::SameLine(); ImGui::Text((char*)glGetString(GL_RENDERER));

		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); hardware_open = false; }

		ImGui::EndPopup();
	}
}

void ModuleInterface::CreateControlsWindow(bool* open)
{
	ImGui::OpenPopup("Controls");
	if (ImGui::BeginPopupModal("Controls"))
	{
		ImGui::Text("Use 'Right Click +WASD' to move arround the scene."), ImGui::Separator();
		ImGui::Text("Use 'Q' to move upwards and 'E' to move downwards."), ImGui::Separator();
		ImGui::Text("Use 'Alt' & Left Click to orbit arround the object."), ImGui::Separator();
		ImGui::Text("Use 'Mouse Wheel' to zoom In and Out."); ImGui::Separator();
		ImGui::Text("Use Mouse Middle Button to move the camera."); ImGui::Separator();
		ImGui::Text("Use 'F' to focus the camera on the object."); ImGui::Separator();
		ImGui::Text("Use 'Shift' to duplicate movement speed."); ImGui::Separator();

		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); controls_open = false; }
		ImGui::EndPopup();
	}
}

void ModuleInterface::CreateSaveSceneWindow(bool* open)
{
	string name;
	char sceneName[50] = "";
	ImGui::OpenPopup("Save your scene");
	if (ImGui::BeginPopupModal("Save your scene"))
	{
		name = sceneName;
		ImGui::InputText("Name", sceneName, IM_ARRAYSIZE(sceneName), ImGuiInputTextFlags_AutoSelectAll);
		if (ImGui::Button("Save") || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			App->scene->SaveScene(name.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) 
		{ 
			ImGui::CloseCurrentPopup(); 
			save_scene_open = false; 
		}
		ImGui::EndPopup();
	}
}
