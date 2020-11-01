#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetLightSlider(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
	this->brightness = brightness;
}

void ModuleWindow::SetWindowSize(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
	this->height = height, this->width = width;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
	{
		SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		original_w = App->window->width;
		original_h = App->window->height;

		SDL_DisplayMode dM;
		SDL_GetDesktopDisplayMode(0, &dM);
		SDL_SetWindowSize(App->window->window, dM.w, dM.h);
		App->renderer3D->OnResize(dM.w, dM.h);
	}
	else
	{
		SDL_SetWindowSize(App->window->window, original_w, original_h);
		App->renderer3D->OnResize(original_w, original_h);
		SDL_SetWindowFullscreen(App->window->window, 0);
	}
}

void ModuleWindow::SetResizable(bool resizable)
{
	resizable ? SDL_WINDOW_RESIZABLE, SDL_SetWindowResizable(window, (SDL_TRUE)) : 0, SDL_SetWindowResizable(window, (SDL_FALSE));
	this->resizable = resizable;
	SDL_GetWindowSize(window, &width, &height);
}

void ModuleWindow::SetBorderless(bool borderless)
{
	borderless ? SDL_SetWindowBordered(window, SDL_FALSE) : SDL_SetWindowBordered(window, (SDL_TRUE));
	this->borderless = borderless;
	SDL_GetWindowSize(window, &width, &height);
}

void ModuleWindow::SetFullDesktop(bool fulldesktop)
{
	Uint32 flags = fulldesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
	SDL_SetWindowFullscreen(window, flags);
	this->fulldesktop = fulldesktop;
	SDL_GetWindowSize(window, &width, &height);
}