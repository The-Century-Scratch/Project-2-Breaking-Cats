#pragma once

#include "Scene.h"
#include "Fonts.h"
#include "EASTL/list.h"
#include "Unit.h"
#include <memory>
#include <vector>

class Player;
class Enemy;
class BattleMenu;
class SceneGameplay;
class Map;
class Inventory;
class ParticlesManager;
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

public:
	eastl::list<Enemy*> enemyList;
	eastl::list<Player*> playerList;

private:
	int numEnemies;

	std::vector<std::unique_ptr<Unit>> units;

	int turn = 0;
	int pauseMenu;
	int turnTimer;


	Unit unit;

	// Reference for the scene
	SceneGameplay* scene;
	Font* font;
	ParticlesManager* particles;
	Map* map;
	// Menu
	BattleMenu* battleMenu;

	SString combatTheme;
};