#pragma once

#include "Item.h"

class MysticalEnergy : public Item
{
public:
	MysticalEnergy(iPoint pos, SDL_Texture* atlas);
	virtual ~MysticalEnergy();

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
