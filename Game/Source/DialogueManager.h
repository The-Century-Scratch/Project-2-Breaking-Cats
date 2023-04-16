#ifndef __DialogueManager_H__
#define __DialogueManager_H__

#define MAX_CHARACTERS_IN_LINE 54

#include "Module.h"

#include "Scene_Base.h"
#include "TextureManager.h"

#include "Defs.h"		//StringHash
#include <list>
#include <vector>

#include <iostream>
#include <queue>

//#include "List.h"
//#include <SDL_ttf/include/SDL_ttf.h>

using namespace std;

class Dialogue {

public:

	Dialogue() {
		sentenceList = new std::list<std::string>;
	}
	Dialogue(std::list<std::string>* sentences) {
		sentenceList = sentences;
	}
	~Dialogue() {}

public:
	std::list<std::string>* sentenceList;

};

enum DialogueProgress {

	GREETING,
	QUESTION,
	ANSWERA,
	ANSWERB,
	GREETINGA,
	GREETINGB

};

class DialogueManager : public Module
{
public:

	DialogueManager();

	// Destructor
	~DialogueManager() final;

	// Called before render is available
	bool Awake(pugi::xml_node& config) final;

	// Called before the first frame
	bool Start() final;

	// Called before all Updates
	bool PreUpdate() final;

	bool Pause(int phase) final;

	// Called each loop iteration
	bool Update(float dt) final;

	// Called before all Updates
	bool PostUpdate() final;

	// Called before quitting
	bool CleanUp() final;

	bool HasSaveData() const final;
	bool LoadState(pugi::xml_node const& data) final;
	pugi::xml_node SaveState(pugi::xml_node const&) const final;

	bool DrawTextbox();

	bool DrawDialogText(std::string s, uint yoffset);

	bool SetDialogue(Dialogue dialogue);

	bool UpdateDialogue();

	bool ResetDialogue();
	
	
	//dialogue things

	bool dialogueEnabled;
	std::list<std::string>* sentenceQueue;
	//std::_List_iterator<std::string>* sentenceQueue;
	std::string playerName;
	uint rows = 3;
	SDL_Rect textbox;
	int fontId;
	DialogueProgress progress;
	int questionEnabled;

	Dialogue firstQuestion;
	Dialogue firstOption;
	Dialogue answerA;
	Dialogue answerB;
	Dialogue firstQuestionA;
	Dialogue firstQuestionB;
};

#endif // __DialogueManager_H__