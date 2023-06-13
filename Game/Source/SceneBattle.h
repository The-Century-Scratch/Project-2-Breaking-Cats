#pragma once

#include "Scene.h"
#include "Fonts.h"
#include "EASTL/list.h"
#include "EASTL/vector.h"
#include "EASTL/unique_ptr.h"
#include "Unit.h"
#include "GridSystem.h"
#include <memory>
#include <vector>

class Player;
class Enemy;
class BattleMenu;
class SceneGameplay;
class Map;
class Inventory;
class ParticlesManager;

struct LoadedUnit
{
	int x;
	int y;
	int type;
};

class SceneBattle : public Scene
{
public:
	SceneBattle();
	virtual ~SceneBattle();

	// Called before the first frame
	bool Load() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	void Draw();

	// Called before quitting
	bool UnLoad() override;

	/*bool LoadState(pugi::xml_node&) { return true; }

	bool SaveState(pugi::xml_node&) const { return true; }*/


private:
	eastl::vector<eastl::unique_ptr<Unit>> units;
	//eastl::vector<pugi::xml_node> nodes;

	int turn = 0;
	int turnTimer;

	UnitType actualTurnUnit;


	eastl::unique_ptr<GridSystem> gridSystem;

	SString combatTheme;
};