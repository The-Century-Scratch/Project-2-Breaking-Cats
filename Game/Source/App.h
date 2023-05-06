#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

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
class QuestManager;
class GuiManager;
class Hud;

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


	// Load / Save
	bool LoadGame();
	bool SaveGame() const;
public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	SceneManager* sceneManager;
	AssetsManager* assetsManager;
	ModuleCollisions* moduleCollisions;
	Map* map;
	EntityManager* entityManager;
	QuestManager* questManager;
	GuiManager* guiManager;
	Hud* hud;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// xml_document to store the config file and xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	// Save file
	pugi::xml_document saveLoadFile;
	pugi::xml_node saveState;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	uint frames;
	float dt;
};

extern App* app;

#endif	// __APP_H__