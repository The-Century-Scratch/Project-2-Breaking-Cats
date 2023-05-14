#ifndef __UNIT_H__
#define __UNIT_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Player.h"


class Unit : public Sprite, public Transform
{
public:
	struct PlayerAction
	{
		enum class Action
		{
			NONE = 0x0000,
			MOVE = 0x0001,
			INTERACT = 0x0002,
			ATTACK = 0x0003,
			ATTACK_LONG_RANGE = 0x0004,
			ATTACK_TO_PLAYER = 0x0005,
			ATTACK_TO_PLAYER_LONG_RANGE = 0x0006,
			PREPARE_DASH = 0x0007,
			ATTACK_AND_HEAL_WITH_KILL = 0x0008
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
	virtual int GetDamage();

	virtual void Update();

	int velocity = 0;
	
	virtual bool GetIsAlly();

	pugi::xml_node parameters;

private:
	void AnimateMove();
	void SmoothMove();
	void StartMovement();

	int moveTimer = 0;
	iPoint moveVector = { 0,0 };
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
	int healthPoints = 1;
	int damage;

	SDL_Rect currentSpriteSlice{ 0 };
};

#endif //__PLAYER_H__
