#ifndef __GATS_H__
#define __GATS_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Gats : public Unit
{
public:
	Gats();
	~Gats();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const override;
	void StartAction(PlayerAction playerAction) override;

	bool GetHasFinishedTurn() override;
	bool GetIsMyTurn() override;

	bool GetIsAlly() override;

	void SetHasFinishedTurn(bool value) override;
	void SetIsMyTurn(bool value) override;

	int GetHealthPoints() override;
	void DealDamage(int amount) override;
	int GetDamage() override;

	//void Update() override;

	void Test();

	//pugi::xml_node parameters;

private:
	const int timeForATile = 2;
	const int tileSize = 16;
	bool goingToDash = false;

	
	int animTimer = 0;
	//int texture;
	SDL_Texture* texture;
	SString name;
	const char* texturePath;

	int healthPoints = 50;
	int damage = 10;
	int maxHealth = 50;

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
