#ifndef __PANELASSETS_H__
#define __PANELASSETS_H__

#include "Panel.h"
#include "Timer.h"
#include <list>
#include <stack>

class PanelAssets : public Panel
{
public:
	PanelAssets(const char* name);
	~PanelAssets();

	void Draw();

	void pushElement(assetsElement* element);

	void clearElements();
	void selectElement(assetsElement* element);

public:

	std::list<assetsElement*> elements;
	assetsElement* selected_element = nullptr;
	std::string current_path = ASSETS_FOLDER;

	bool creating = false;
	bool renaming = false;

	Timer update_timer;
	float update_delay = 1000;
	bool force_update = false;
};

#endif // !__PANELASSETS_H__