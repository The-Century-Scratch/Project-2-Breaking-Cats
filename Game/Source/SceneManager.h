#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"


struct SDL_Texture;


enum class TransitionStep;
class DialogueManager;

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

	bool SaveState(pugi::xml_node&);

	void DrawTransition();

	//int important to know what MAP TO LOAD FROM TILED
	//can also used to know in what scene you are
	int currentScene;
	
	bool Pause = false;
	//variable in city
	bool tabern = false, store = false, resistance_base = false, downAfterLabrinth = false, topPreLab = false;
	//variable in labrinth
	bool village = false, leftAfterLabrinth = false;
	//variable in after labrinth
	bool rightLabrinth = false, nordCity = false;
	//variable in preLab
	bool downCity = false, lab = false;

	//variable to see what puzzles has ben solved
	bool puzzle1solved = false, puzzle2solved = false, puzzle3solved = false;

	bool changeMap = false;

	bool LoadRequestOutScene = false;

	Scene* current;
	Scene* next;
	TransitionStep transitionStep;
	DialogueManager* dialogueManager;
private:
	// Transitions rects

};

#endif // __SCENE_H__