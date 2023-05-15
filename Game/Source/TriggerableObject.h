#ifndef __TRIGGERABLEOBJECT_H__
#define __TRIGGERABLEOBJECT_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class TriggerableObject : public Entity
{
public:

	TriggerableObject(bool triggered = false);
	virtual ~TriggerableObject();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	void EndCollision(Collider* c1, Collider* c2) override;

	SDL_Rect cRect;

	Collider* boundaries;

	//if its in range with player
	bool inRange = false;

	//if it has been triggered
	bool triggered;

	int id;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnim;
	Animation untriggeredAnim;
	Animation triggeredAnim;
};

#endif // __TRIGGERABLEOBJECT_H__