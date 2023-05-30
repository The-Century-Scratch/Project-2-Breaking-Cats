#pragma once
#ifndef __MOVEROCKQUEST_H__
#define __MOVEROCKQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MoveRockQuest: public Quest
{
public:
	MoveRockQuest(pugi::xml_node node);
	virtual ~MoveRockQuest();

	bool Update() override;
	void Draw(Font* font) override;

public:
	bool completed = false;
	int npcId;
};

#endif // __MOVEROCKQUEST_H__