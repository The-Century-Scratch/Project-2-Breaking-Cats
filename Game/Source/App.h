#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"
#include "EASTL/unique_ptr.h"
#include "EASTL/list.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class SceneManager;
class AssetsManager;
class TextureManager;
class ModuleCollisions;
class Map;
class EntityManager;
class ParticleSystemManager;
class QuestManager;
class GuiManager;
class StatsManager;
class Hud;
class Inventory;
class InventoryShop;
class Debug;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// Methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;
	// Load / Save
	bool LoadGame();
	bool SaveGame() const;
	// Load config file to var
	pugi::xml_node LoadConfigFileToVar();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();



public:

	// Modules
	eastl::unique_ptr<Window> win;
	eastl::unique_ptr<Input> input;
	eastl::unique_ptr<Render> render;
	eastl::unique_ptr<Textures> tex;
	eastl::unique_ptr<Audio> audio;
	eastl::unique_ptr<SceneManager> sceneManager;
	eastl::unique_ptr<AssetsManager> assetsManager;
	eastl::unique_ptr<ModuleCollisions> moduleCollisions;
	eastl::unique_ptr<Map> map;
	eastl::unique_ptr<EntityManager> entityManager;
	eastl::unique_ptr<ParticleSystemManager> particleManager;
	eastl::unique_ptr<QuestManager> questManager;
	eastl::unique_ptr<GuiManager> guiManager;
	eastl::unique_ptr<StatsManager> statsManager;
	eastl::unique_ptr<Hud> hud;
	eastl::unique_ptr<Inventory> inventory;
	eastl::unique_ptr<InventoryShop> inventoryShop;
	eastl::unique_ptr<Debug> debug;

	bool frcap = true;

private:

	int argc = 0;
	char** args;
	SString title;
	SString organization;

	eastl::list<Module *> modules;

	// xml_document to store the config file and xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	// Save file
	pugi::xml_document saveLoadFile;
	pugi::xml_node saveState;

	mutable bool saveGameRequested = false;
	bool loadGameRequested = false;

	uint frames = 0;
	float dt = 0;

	Timer timer;
	PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;

	uint32 maxFrameDuration = 0;
};

extern App* app;

#endif	// __APP_H__