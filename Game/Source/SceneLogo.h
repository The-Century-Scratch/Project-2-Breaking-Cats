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
	float Pos;
	
	Animation logoAnimation;
	Animation logoAnimationStatic;

	Animation* currentAnimation;

	int timer;
};

#endif //__SCENELOGO_H__