#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInterface.h"
#include "ModuleLoader.h"

#include "UI_Config.h"
#include "UI_Scene.h"
#include "UI_Hierarchy.h"
#include "UI_Console.h"
#include "UI_Inspector.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleInterface* ui;
	ModuleLoader* loader;

	UI_Config* configuration = nullptr;
	UI_Scene* scene_ui = nullptr;
	UI_Hierarchy* hierarchy = nullptr;
	UI_Console* console = nullptr;
	UI_Inspector* inspector = nullptr;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;
	list<UI_Element*> list_UImodules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(char* url);
	void ExitApp();

	float milliseconds;

private:

	void AddModule(Module* mod);
	void AddUIModule(UI_Element* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;