#ifndef __GRIDSYSTEM_H__
#define __GRIDSYSTEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class GridSystem
{
public:

	GridSystem();
	virtual ~GridSystem();

	bool Start();

	bool Update();

	void Draw();

	bool CleanUp();

private:
};

#endif // __GRIDSYSTEM_H__