#ifndef __QUEST_H__
#define __QUEST_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class QuestType
{
	MAINQUEST,
	SIDEQUEST
};

class Quest
{
public:
	Quest() {};
	Quest(QuestType type) : type(type) {}

	virtual bool Update() { return true; }

	virtual void Draw(Font* font) {}

public:

	SString name;
	SString description;
	QuestType type;
	int id;
	int nextQuestId;
	int reward;
};

#endif // __QUEST_H__