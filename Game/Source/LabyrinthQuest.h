#ifndef __LABYRINTHQUEST_H__
#define __LABYRINTHQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class LabyrinthQuest : public Quest
{
public:
	LabyrinthQuest(pugi::xml_node node);
	virtual ~LabyrinthQuest();

	bool Update() override;
	void Draw(Font* font) override;

public:

	int npcId;
};

#endif // __LABYRINTHQUEST_H__