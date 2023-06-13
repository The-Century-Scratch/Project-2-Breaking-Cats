#ifndef __COLLECTIBLEOBJECT_H__
#define __COLLECTIBLEOBJECT_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class CollectibleObject : public Entity
{
public:

	CollectibleObject(bool solved = false);
	virtual ~CollectibleObject();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	SDL_Rect cRect;

	int id;

	bool solved = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation anim;
};

#endif // __COLLECTIBLEOBJECT_H__