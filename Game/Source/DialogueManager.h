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

enum MICEY {
	M_GREETING,
	M_ANSWER,
	M_REPLICATION1,
	M_REPLICATION2,
	M_END
};

enum  CONTRABANDIST {
	C_GREETING,
	C_ANSWER,
	C_REPLICATION1,
	C_REPLICATION2,
	C_RETURNAL1,
	C_RETURNAL2,
};

enum WARD {
	W_CONVERSATION,
	W_RETURNAL
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
	bool questionEnabled;
	bool miceyDialog, contrabandistDialog, wardDialog, signDialog;

	std::list<std::string>* sentenceQueue;
	//std::string playerName;
	uint rows = 3;
	SDL_Rect textbox;

	MICEY micey;
	CONTRABANDIST contrabandist;
	WARD ward;

	//MICEY
	Dialogue M_greeting;
	Dialogue M_answer;
	Dialogue M_replicationA;
	Dialogue M_replicationB;
	Dialogue M_returnalA;
	Dialogue M_returnalB;
	
	//WARD
	Dialogue W_conversation;
	Dialogue W_returnal;
	//SIGN
	Dialogue S_text;

	//CONTRABANDIST
	Dialogue C_greeting;
	Dialogue C_answer;
	Dialogue C_replicationA;
	Dialogue C_replicationB;
	Dialogue C_returnalA;
	Dialogue C_returnalB;




};

#endif // __DialogueManager_H__