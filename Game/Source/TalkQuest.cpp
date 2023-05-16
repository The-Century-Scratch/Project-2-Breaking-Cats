#include "TalkQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"

TalkQuest::TalkQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->npcId = node.attribute("npcId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::TALK;
}

TalkQuest::~TalkQuest() {}

bool TalkQuest::Update() {
	bool ret = true;

	return ret;
}

void TalkQuest::Draw(Font *font) {
	app->render->DrawText(font, description.GetCharString(), 50, 50, 48, 5, { 255,255,255,255 }, 528);
}