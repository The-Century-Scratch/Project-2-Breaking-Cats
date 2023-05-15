#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "Point.h"
#include "Player.h"
#include "NPC.h"
#include "MovableObject.h"
#include "TriggerableObject.h"
#include "StaticObject.h"
#include "List.h"
//#include "ParticlesManager.h"

//#include "Font.h"
#include "EASTL/list.h"

class Player;
class Enemy;
class CharacterManager;
class SceneBattle;
class EntityManager;
class Map;
class PauseMenu;
class Render;
class Item;
class Inventory;
class QuestManager;
class QuestMenu;
class Shop;

class Font;


//TO CITY
#define LEAVETABERN iPoint(298,104)
#define LEAVESTORE iPoint(568,714)
#define LEAVEBASE iPoint(136,440)
#define LEAVEAFTERLABRINTHDOWN iPoint(389, 8)
#define LEAVEPRELABTOP iPoint(384, 748)

//TO LABRINTH
#define LEAVEVILLAGE iPoint(6, 150)
#define LEAVEAFTERLABRINTHLEFT iPoint(462, 150)

//TO VILLAGE
#define LEAVELABRINTHLEFT iPoint(620, 125)

//TO AFTERLABRINTH
#define LEAVELABRINTHRIGHT iPoint(5, 159)
#define LEAVECITYTOP iPoint(243, 297)

//TO PRELAB
#define LEAVECITYDOWN iPoint(208, 6)
#define LEAVELAB iPoint(379, 294)

#define IDSCENEMAP 0
#define IDSCENEBASE 1
#define IDSCENESTORE 2
#define IDSCENETABERN 3
#define IDLABRINTH 4
#define IDAFTERLABRINTH 5
#define IDVILLAGE 6
#define IDPRELAB 7

#define INIT_POS_TABERN iPoint(334,106)
#define INIT_POS_BASE iPoint(113,366)
#define INIT_POS_STORE iPoint(71,173)


enum class PlayerType;
enum class EntityType;

enum class GameplayMenuState
{
	NONE = 0,
	CHARACTER_SWAP,
	INVENTORY,
	PAUSE,
	QUESTS,
	SHOP
};

enum class GameplayState
{
	NONE = 0,
	ROAMING,
	BATTLE,
	DIALOG,
};

class SceneGameplay : public Scene
{
public:
	SceneGameplay();

	virtual ~SceneGameplay() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	void ChangeState(GameplayMenuState type);

	void CharacterSwap(PlayerType pType);
	
	bool CollisionMapEntity(SDL_Rect rect, EntityType type);

	void ChangeMap(iPoint newPos, int newScene = 1);
private:
	void HandleInput(Input* input, float dt);
	
	bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2);
	void GenerateBattle();
	void CameraFollow(Render* render);
	void Transitioning(float dt);
	void ChangeBlockBounds(int bounds_x, int bounds_y);
	void SetCameraMovement(int target_x, int target_y, float dt);
	void DrawDebugVariable();

	void LoadMovableObjects();
	void LoadTriggerableObjects();
	void LoadStaticObject();
	void LoadNpc();
	void LoadItems(pugi::xml_node& n);

	void UpdatingButtons(Input* input);

private:
	Player* currentPlayer;
	List<NPC*> npcs;
	List<MovableObject*> movableObjectList;
	List<TriggerableObject*> triggerableObjectList;
	List<StaticObject*> staticObjectList;

	SDL_Texture* goldTexture;
	eastl::list<Player*> playerList;

	// Menus
	CharacterManager* charManager;
	PauseMenu* pause;
	GameplayMenuState menuState;
	GameplayState gameState;

	EntityManager* entityManager;

	//ParticlesManager* particles;

	QuestManager* questManager;

	// Scenes
	SceneBattle* sceneBattle;
	float temp;

	Map* map;

	// Font
	Font* font;

	// Inventory
	Inventory* inventory;

	// Quest Menu
	QuestMenu* quests;

	// Shop Menu
	Shop* shop;

	// Interruptor block
	Item* interruptorBlock;

	SDL_Texture* itemText;
	SDL_Texture* guiTex;
	SDL_Texture* guiPad;

	List<Item*> items;

	Item* firePaw;
	Item* dragonSlayer;
	Item* grapplingHook;
	Item* bulletPenetration;
	Item* mysticalEnergy;
	Item* arcaneSpirit;

	iPoint tmpPosPlayer;

	float alpha;
	bool transition;
	bool fadeOut;
	bool loadObjects;
	bool deleteDoor;
	//Fx
	int doorOpenedFx;
	int doorClosedFx;

	int puzzleCompletedFx;
	bool canSound1;
	bool canSound2;
	bool canSound3;
	bool canSound4;
	
	int whereMove;
	int cameraCounter;

	eastl::list<Enemy*> enemyList;
	Enemy* tmp;

	SDL_Rect iceBlockRect;

	float firstQuest;
	bool firstQuestAdded;

	int lastUserInput;

	//Music
	SString cityTheme;
	SString shopTheme;




};

#endif //__SCENEGAMEPLAY_H__