#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"


struct SDL_Texture;

enum class TransitionStep;

class SceneManager : public Module
{
public:

	SceneManager(bool startEnabled);

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&) const;

	//int important to know what MAP TO LOAD FROM TILED
	//can also used to know in what scene you are
	int currentScene;
	
	bool Pause = false;

	Scene* current;
	Scene* next;
	TransitionStep transitionStep;
private:
	// Transitions rects
};

#endif // __SCENE_H__