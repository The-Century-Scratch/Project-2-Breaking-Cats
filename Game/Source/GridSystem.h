#ifndef __GRIDSYSTEM_H__
#define __GRIDSYSTEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"
#include "EASTL/list.h"
#include "EASTL/vector.h"
#include "EASTL/unique_ptr.h"
#include "Unit.h"

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
	TileWalkability walkability;
	TileState state;
	bool isFocused;
};

struct UnitData
{
	iPoint unitPos;
	UnitType type;
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

	void LoadUnitData(Unit* u);

	bool isWalkable(iPoint pos);

	bool isUnit(iPoint pos);

	bool isPortal(iPoint pos);

	void move(iPoint origin, iPoint destination);

	void removeUnit(iPoint UnitPos);

	void showActionArea();

	bool AreaIsClicked();

	eastl::vector<iPoint> getHitsPosition();

	iPoint getFocusPosition();

	void PlacePortal(iPoint pos);


	Unit::PlayerAction currentAction;

	bool showArea;

	iPoint lastPortal, firstPortal;
private:

	bool IsMouseInside(SDL_Rect r);

	void HandleTileState();

	void showEffectArea(SDL_Rect r);

	void DrawTileState();

	TileWalkability LoadWalkabilityfromCollisions(iPoint pos);

	void showAttack(iPoint pos);

	void showAttackRange(iPoint pos);

	void showDash(iPoint pos);

	void showGrenade(iPoint pos);

	void showTeleport(iPoint pos);

	void showPortal(iPoint pos);

	void showAttackAOE(iPoint pos);

	void showAttackRangeAOE(iPoint pos);

	void showDashAOE(iPoint pos);

	void showGrenadeAOE(iPoint pos);

	void showTeleportAOE(iPoint pos);

	void showPortalAOE(iPoint pos);

	SDL_Texture* gridTex;
	SDL_Rect clickableSection;
	SDL_Rect areaSection;

	Animation focusedAnim;
	Animation* currentAnim = nullptr;

	eastl::vector<eastl::unique_ptr<UnitData>> unitsData;

	Tile grid[MAX_TILES_X][MAX_TILES_Y];
	iPoint gridPos;
	SDL_Rect focusPos;

};

#endif // __GRIDSYSTEM_H__