#ifndef __MODULEINTERFACE_H__
#define __MODULEINTERFACE_H__

#include "Module.h"
#include "Globals.h"

class ModuleInterface : public Module
{
public:
	ModuleInterface(Application* app, bool start_enabled = true);
	~ModuleInterface();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CreateAboutWindow(bool* open);
	void CreateConfigWindow(bool* open);
	void CreateHardwareWindow(bool* open);

public:
	bool about_open = false;
	bool config_open = true;
	bool hardware_open = false;

private:
	char* RDTSC;
	char* MMX;
	char* AVX;
	char* SSE;
	char* SSE2;
	char* SSE3;
	char* SSE41;
	char* SSE42;

private:
	bool show_demo_window = false;
};
#endif
