#ifndef __CATSKA_H__
#define __CATSKA_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Catska : public Unit
{
public:
	Catska();
	~Catska();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

	bool GetHasFinishedTurn() override;
	bool GetIsMyTurn() override;

	bool GetIsAlly() override;

	void SetHasFinishedTurn(bool value) override;
	void SetIsMyTurn(bool value) override;

	int GetHealthPoints() override;
	void DealDamage(int amount) override;
	int GetDamage() override;

	void Update() override;

	//pugi::xml_node parameters;

private:
	void AnimateMove();
	void SmoothMove();
	void StartMovement();

	int moveTimer = 0;
	iPoint moveVector = { 0, 0 };
	int speed = 4;
	const int timeForATile = 2;
	const int tileSize = 16;

	int animTimer = 0;
	//int texture;
	SDL_Texture* texture;
	SString name;

	const char* texturePath;

	bool isMyTurn = false;
	bool hasFinishedTurn = false;
	int healthPoints = 40;
	int damage = 6;

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
