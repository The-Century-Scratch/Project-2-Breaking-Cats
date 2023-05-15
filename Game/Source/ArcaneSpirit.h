#pragma once

#include "Item.h"

class ArcaneSpirit : public Item
{
public:
	ArcaneSpirit(iPoint pos, SDL_Texture* atlas);
	virtual ~ArcaneSpirit();

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