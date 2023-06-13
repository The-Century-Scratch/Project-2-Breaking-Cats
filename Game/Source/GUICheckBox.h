#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
//#include "Animation.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheckBox();

	bool Start();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY = 0;
	unsigned int click = 0;

	bool canClick = true;
	bool drawBasic = false;

private:

	bool checked = 0;
	bool fxDone = false;

};

#endif // __GUICHECKBOX_H__