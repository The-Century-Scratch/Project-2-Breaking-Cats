#ifndef __GRIDSYSTEM_H__
#define __GRIDSYSTEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

#define MAX_TILES_X 9
#define MAX_TILES_Y 9
#define TILE_W 16
#define TILE_H 16

struct SDL_Texture;
class Input;

enum class TileState
{
	DISABLED = 0,
	UNSELECTED,
	CLICKABLE,
	AREA_EFFECT
};

enum class TileWalkability
{
	UNWALKABLE,
	WALKABLE,
	OBSTACLE,
	UNIT
};

struct Tile
{
	SDL_Rect bounds;
	TileWalkability walkablility;
	TileState state;
	bool isFocused;
};

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

	bool IsMouseInside(SDL_Rect r);

	void HandleTileState();

	void DrawTileState();

	SDL_Texture* gridTex;
	SDL_Rect clickableSection;
	SDL_Rect areaSection;

	Animation focusedAnim;
	Animation* currentAnim = nullptr;



	Tile grid[MAX_TILES_X][MAX_TILES_Y];
	iPoint gridPos;
};

#endif // __GRIDSYSTEM_H__