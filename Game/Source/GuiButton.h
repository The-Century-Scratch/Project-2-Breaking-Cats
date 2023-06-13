#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text = nullptr, Font* font = nullptr);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX = 0, mouseY = 0;
	unsigned int click = 0;

	bool canClick = true;
	bool drawBasic = false;

	bool fxDone = false;

	Font* font = nullptr;

};

#endif // __GUIBUTTON_H__