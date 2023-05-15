#ifndef __STATICOBJECT_H__
#define __STATICOBJECT_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class StaticObject : public Entity
{
public:

	StaticObject();
	virtual ~StaticObject();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	SDL_Rect cRect;

	//if it has been triggered
	bool triggered;

	int id;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation anim;
};

#endif // __STATICOBJECT_H__