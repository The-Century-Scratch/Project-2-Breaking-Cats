#ifndef __GUARDIAN_H__
#define __GUARDIAN_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Guardian : public Unit
{
public:
	Guardian();
	~Guardian();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const override;
	void StartAction(PlayerAction playerAction) override;

	bool GetHasFinishedTurn() override;
	bool GetIsMyTurn() override;

	void SetHasFinishedTurn(bool value) override;
	void SetIsMyTurn(bool value) override;

	bool GetIsAlly() override;

	int GetHealthPoints() override;
	void DealDamage(int amount) override;

	void Update() override;

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

	int goingTo;

	bool isMyTurn = false;
	bool hasFinishedTurn = false;
	int healthPoints = 20;

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
