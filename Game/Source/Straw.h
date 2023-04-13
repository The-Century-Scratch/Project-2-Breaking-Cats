#ifndef __STRAW_H__
#define __STRAW_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Straw : public Unit
{
public:
	struct PlayerAction
	{
		enum class Action
		{
			NONE = 0x0000,
			MOVE = 0x0001,
			INTERACT = 0x0002
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

		Action action = Straw::PlayerAction::Action::NONE;
	};

	Straw();
	~Straw();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	virtual void StartAction(PlayerAction playerAction);

	bool GetHasFinishedTurn();
	bool GetIsMyTurn();

	void SetHasFinishedTurn(bool value);
	void SetIsMyTurn(bool value);

	void Update();

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

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
