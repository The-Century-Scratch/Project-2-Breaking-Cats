#pragma once
#ifndef __MAINQUEST_H__
#define __MAINQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MainQuest: public Quest
{
public:
	MainQuest(pugi::xml_node node);
	virtual ~MainQuest();

	bool Update() override;
	void Draw(Font* font) override;

public:
	bool completed = false;
	int npcId;
};

#endif // __MAINQUEST_H__