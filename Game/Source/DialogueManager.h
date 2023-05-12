#pragma once

#include "Dialogue.h"
#include "Fonts.h"

#include "PugiXml/src/pugixml.hpp"

class Easing;
class QuestManager;
class SceneGameplay;

class DialogueManager
{
public:
	DialogueManager();
	virtual ~DialogueManager();

	bool Start();
	bool Update(float dt);
	void Draw();
	bool UnLoad();

	Dialogue* LoadDialogue(int id); // Load the "npc" node

	NpcNode* LoadNode(int id, pugi::xml_node node);
	NpcNode* GetNodeById(int id);

public:
	Dialogue* current;
	bool printText;
	bool isDialogueActive;
private:
	SDL_Texture* texture;
	QuestManager* questManager;
	SceneGameplay* scene;

	// Pugi stuff to load dialogues
	pugi::xml_document file;
	pugi::xml_node root;

	eastl::list<Dialogue*> dialogues;

	Font* font;

	Easing* easingArrow;
	Easing* easingArrow2;
	float arrowPosition;

	int lastUserInput;
	int letterCount;

	int alpha;

	bool currentPressed;
};