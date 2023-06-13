#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Fonts.h"

class MainMenu;
class Easing;
struct SDL_Rect;

enum class TitleState
{
	NONE = 0,
	TITLE,
	FADE,
	MENU
};

class SceneTitle : public Scene
{
public:
	SceneTitle();

	virtual ~SceneTitle() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* img;
	SDL_Texture* berry;
	SDL_Texture* creditsTex;

	SDL_Rect imgRect;

	SDL_Texture* bg;
	SDL_Texture* bgText;
	
	int enterFx = 0;
	int titleFx = 0;

	TitleState state;

	MainMenu* mainMenu;

	Font* font;

	float titleAlpha = 0;
	
	//TITLE EASING
	float the_x = 0;
	float hunter_x = 0;
	float odyssey_y = 0;
	float currIt = 0;
	float totalIt = 0;
	bool titleEasingActivated = 0;
	
	//ENTER EASING
	bool enterEasingActivatedY = 0;
	bool enterEasingActivated2Y = 0;
	bool enterEasingActivatedX = 0;
	bool enterEasingActivated2X = 0;
	bool enterEasingActivated3X = 0;
	bool enterEasingActivated4X = 0;
	float text_x = 0;
	float text_y = 0;
	float currItEnterY = 0;
	float totalItEnterY = 0;
	float currItEnterX = 0;
	float totalItEnterX = 0;

	SDL_Rect section;
	SDL_Rect section2;
	SDL_Rect section3;

	Easing* easing;

	const char* musicPath;
};

#endif //__SCENETITLE_H__