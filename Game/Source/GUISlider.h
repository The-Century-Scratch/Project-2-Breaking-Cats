#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
//#include "Animation.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBounds);
	virtual ~GuiSlider();

	bool Start();

	bool Update(float dt);
	bool Draw(Render* render);

	//bool CleanUp();

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

private:

	bool one, two, three, four;

	SDL_Rect sliderbounds;

	float boundsX, boundsY;
	bool fxDone = false;
};

#endif // __GUISLIDER_H__