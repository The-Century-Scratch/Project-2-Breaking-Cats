#pragma once

#include "Item.h"

class DiggerSniper : public Item
{
public:
	DiggerSniper(iPoint pos, SDL_Texture* atlas);
	virtual ~DiggerSniper();

	bool Start() override;
	bool Update() override;
	bool PostUpdate() override;
	void Draw();
	bool CleanUp() override;

	bool SaveState(pugi::xml_node& n) override;

	void UseItem(Player* player) override;

public:

	//Collider* boundaries;

	int healAmount;
};
