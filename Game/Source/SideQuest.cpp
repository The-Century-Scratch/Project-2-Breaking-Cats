#include "SideQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"
#include "QuestManager.h"

SideQuest::SideQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->npcId = node.attribute("npcId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::SIDEQUEST;
	Coinrect = { 192,0,32,32 };
	rect = { 0,0,0,0 };
}

SideQuest::~SideQuest() {}

bool SideQuest::Update() {
	bool ret = true;

	return ret;
}

void SideQuest::Draw(Font *font) {
	if (this->id != app->questManager->SideQuestID) {
		//title
		app->render->DrawText(font, name.GetCharString(), 80, 115, 80, 5, { 255,255,255,255 }, 1000);
		//description
		app->render->DrawText(font, description.GetCharString(), 50, 250, 48, 5, { 255,255,255,255 }, 1000);
		//rewards
		app->render->DrawText(font, "Rewards:", 1000, 200, 48, 5, { 255,255,255,255 }, 100);
		//rewards tex
		switch (this->id) {
		case 0:
			app->render->DrawTexture(app->inventory->itemTexture, 350, 125, &Coinrect, 0);
			break;
		case 1:
			rect = { 128,0,32,32 };
			app->render->DrawTexture(app->inventory->itemTexture, 350, 125, &rect, 0);
			break;
		default:break;
		}
		//count lo voy a harcodear y me la pela
		switch (app->questManager->ObjectsCount) {
		case 0:
			app->render->DrawText(font, "Count: 0", 780, 550, 48, 5, { 255,255,255,255 });
			break;
		case 1:
			app->render->DrawText(font, "Count: 1", 780, 550, 48, 5, { 255,255,255,255 });
			break;
		case 2:
			app->render->DrawText(font, "Count: 2", 780, 550, 48, 5, { 255,255,255,255 });
			break;
		case 3:
			app->render->DrawText(font, "Count: 3", 780, 550, 48, 5, { 255,255,255,255 });
			break;
		case 4:
			app->render->DrawText(font, "Count: 4", 780, 550, 48, 5, { 255,255,255,255 });
			break;
		default:break; 
		}
	}
	else {
		app->render->DrawText(font, "Not found", 80, 115, 80, 5, { 255,255,255,255 }, 900);
	}
}