#include "LabyrinthQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"

LabyrinthQuest::LabyrinthQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->npcId = node.attribute("npcId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::TALK;
}

LabyrinthQuest::~LabyrinthQuest() {}

bool LabyrinthQuest::Update() {
	bool ret = true;

	////Check completion event
	//ListItem<NPC*>* nitem = app->scene->npcs.start;
	//NPC* npc = nitem->data;
	//while (nitem != nullptr)
	//{

	//	if (npc->npcid == this->npcId) { break; }
	//	nitem = nitem->next;
	//	npc = nitem->data;
	//}

	////Completion event: player is in npc boundaries and talks to him (G key)
	//if ((app->scene->player->tile.x * 32 >= npc->boundaries.x) &&
	//	(app->scene->player->tile.x * 32 < npc->boundaries.x + npc->boundaries.w) &&
	//	(app->scene->player->tile.y * 32 >= npc->boundaries.y) &&
	//	(app->scene->player->tile.y * 32 < npc->boundaries.y + npc->boundaries.h) &&
	//	(app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)) {
	//	ret = false;

	//	app->scene->points += this->reward;
	//}

	return ret;
}

void LabyrinthQuest::Draw(Font* font) {
	app->render->DrawText(font, description.GetCharString(), 50, 50, 48, 5, { 255,255,255,255 }, 528);
}