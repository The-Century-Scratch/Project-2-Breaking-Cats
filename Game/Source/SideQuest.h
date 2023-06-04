#ifndef __SIDEQUEST_H__
#define __SIDEQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class SideQuest : public Quest
{
public:
	SideQuest(pugi::xml_node node);
	virtual ~SideQuest();

	bool Update() override;
	void Draw(Font* font) override;

public:

	int npcId;
};

#endif // __SIDEQUEST_H__