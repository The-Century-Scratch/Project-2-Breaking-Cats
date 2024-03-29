#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"

class Easing;

class SceneEnding : public Scene
{
public:
	SceneEnding(bool win);

	virtual ~SceneEnding() {}

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