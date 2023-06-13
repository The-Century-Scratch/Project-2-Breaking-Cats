#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"
#include "Animation.h"

class Easing;

class SceneLogo : public Scene
{
public:
	SceneLogo();

	virtual ~SceneLogo() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* logo;
	Easing* easing;
	float Pos = 0;
	
	Animation logoAnimation;
	Animation logoAnimationStatic;

	Animation* currentAnimation;

	int timer = 0;

	uint logofx = 0;
};

#endif //__SCENELOGO_H__