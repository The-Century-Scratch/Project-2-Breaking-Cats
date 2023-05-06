#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneManager.h"
#include "ModuleCollisions.h"
#include "Map.h"
#include "EntityManager.h"
#include "QuestManager.h"
#include "AssetsManager.h"
#include "GuiManager.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

////EASTL SETUP
//void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
//{
//	return new uint8_t[size];
//}

// EASTL library
void* operator new[](size_t size, const char* pName, int flags, unsigned	 debugFlags, const char* file, int line)
{
	return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return malloc(size);
}


// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	sceneManager = new SceneManager(true);
	moduleCollisions = new ModuleCollisions(true);
	map = new Map(true);
	entityManager = new EntityManager(false);
	questManager = new QuestManager(true);
	guiManager = new GuiManager(true);
	hud = new Hud(true);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(sceneManager);
	AddModule(moduleCollisions);
	AddModule(hud);
	AddModule(guiManager);
	AddModule(entityManager);
	AddModule(questManager);
	AddModule(map);

	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	//Load config from XML
	bool ret = LoadConfig();

	if(ret == true)
	{
		//Read the title from the config file
		title = configNode.child("app").child("title").child_value();

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled()) { ret = item->data->Start(); }
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// This is a good place to call Load / Save functions
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->state == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;

	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->state == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->state == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled() == true) { ret = item->data->CleanUp(); }
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

pugi::xml_node App::LoadConfigFileToVar()
{
	pugi::xml_node ret;
	// Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// Check result for loading errors
	if (parseResult) {
		ret = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}



// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	loadGameRequested = false;
	bool ret = true;

	pugi::xml_parse_result resul = saveLoadFile.load_file("save_game.xml");

	if (resul == NULL)
	{
		LOG("Could not load save and load xml file. pugi error: %s", resul.description());
		return false;
	}
	else
	{
		saveState = saveLoadFile.child("save_status");

		//eastl::list<Module*>::iterator item = modules.begin();
		//List<Module*>* item;
		//item = modules.start;

		ListItem<Module*>* item;
		item = modules.start;

		while (item != modules.end && ret)
		{
			ret = item->data->LoadState(saveState.child(item->data->name.GetString()));
			++item;
		}

		LOG("File loaded successfully!");
	}

	return ret;
}

bool App::SaveGame() const
{
	LOG("Saving Results!!");
	saveGameRequested = false;
	bool ret = true;

	//eastl::list<Module*>::iterator item = modules.begin().mpNode;

	ListItem<Module*>* item = modules.start;

	pugi::xml_document file;

	auto root = file.append_child("save_status");

	while (item != modules.end)
	{
		root.append_child(item->data->name.GetString());
		ret = item->data->SaveState(root.child(item->data->name.GetString()));
		++item;
	}

	bool saveSucceed = file.save_file("save_game.xml", PUGIXML_TEXT("  "));
	if (saveSucceed == false)
	{
		LOG("Couldn't save the file. pugi error: %s", pugi::status_internal_error);
	}

	return ret;
}