#ifndef __MOVABLEOBJECT_H__
#define __MOVABLEOBJECT_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MovableObject : public Entity
{
public:

	MovableObject(bool solved = false);
	virtual ~MovableObject();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	SDL_Rect cRect;

	int id = 0;

	bool solved = false;
	iPoint solvedPosition;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation anim;
};

#endif // __MOVABLEOBJECT_H__