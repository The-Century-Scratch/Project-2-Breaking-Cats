#ifndef __SCENECREDITS_H__
#define __SCENECREDITS_H__

#include "Scene.h"

class Easing;

class SceneCredits : public Scene
{
public:
	SceneCredits();

	virtual ~SceneCredits() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	//SDL_Texture* guiTexture;

	SDL_Texture* bg;
	bool hasWin;

	Easing* easingUp;
	Easing* easingDown;
	float easingPosition;
};

#endif //__SCENEENDING_H__