#ifndef __UNIT_H__
#define __UNIT_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Player.h"


class Unit : public Player
{
public:
	struct PlayerAction
	{
		enum class Action
		{
			NONE = 0x0000,
			MOVE = 0x0001,
			INTERACT = 0x0002,
			ATTACK = 0x0003
		};

		friend Action operator&(Action a, Action b)
		{
			return static_cast<Action>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
		}

		friend Action operator&=(Action& a, Action b)
		{
			return a = a & b;
		}

		friend Action operator|(Action a, Action b)
		{
			return static_cast<Action>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
		}

		friend Action& operator|=(Action& a, Action b)
		{
			return a = a | b;
		}


		iPoint destinationTile = { 0, 0 };

		Action action = Unit::PlayerAction::Action::NONE;
	};

	Unit();
	~Unit();
	virtual void Create(iPoint pos);

	virtual void Draw() const;
	virtual void DebugDraw() const;

	virtual PlayerAction HandleInput() const;
	virtual void StartAction(PlayerAction playerAction);

	virtual bool GetHasFinishedTurn();
	virtual bool GetIsMyTurn();

	virtual void SetHasFinishedTurn(bool value);
	virtual void SetIsMyTurn(bool value);

	virtual void DealDamage(int amount);
	virtual int GetHealthPoints();

	virtual void Update();

	int velocity = 0;
	
	virtual bool GetIsAlly();

private:
	void AnimateMove();
	void SmoothMove();
	void StartMovement();

	int moveTimer = 0;
	iPoint moveVector{ 0 };
	int speed = 4;
	const int timeForATile = 2;
	const int tileSize = 16;

	int animTimer = 0;
	int texture;

	bool isMyTurn = false;
	bool hasFinishedTurn = false;
	int healthPoints = 1;

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
