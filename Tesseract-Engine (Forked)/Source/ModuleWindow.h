#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_File* document);
	bool CleanUp();

	void SetTitle(const char* title);

	bool SaveDefaultConfig(JSON_File* document)const;

	void OnResize(int width, int height);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	int height=0;
	int width=0;
	int screen_size=0;
	int screen_margin_w = 0;
	int screen_margin_h = 0;
	
	bool fullscreen=false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

};

#endif // __ModuleWindow_H__