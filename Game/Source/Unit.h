#ifndef __UNIT_H__
#define __UNIT_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Player.h"
#include "EASTL/unique_ptr.h"

#define FACING_LEFT 0
#define FACING_RIGHT 1

class Easing;

enum class ActionState
{
	IDLE,
	ATTACK,
	DIE
};
enum class UnitType
{
	UNDEFINED,
	GATS,
	CATSKA,
	GUARDIAN,
	LONGRANGE,
	STRAW
};

class Unit : public Transform
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
			ATTACK_AND_HEAL_WITH_KILL = 0x0008,
			GRENADE = 0x0009
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
	void Create(iPoint pos);

	virtual void Draw() const;
	virtual void DebugDraw() const;

	virtual PlayerAction HandleInput() const;
	virtual void StartAction(PlayerAction playerAction);

	bool GetHasFinishedTurn();
	bool GetIsMyTurn();

	void SetHasFinishedTurn(bool value);
	void SetIsMyTurn(bool value);

	void DealDamage(int amount);
	int GetHealthPoints();
	int GetDamage();
	int GetPlayerId();
	SString GetName();
	UnitType GetType();
	bool GetIsAlly();

	void Update();

	int velocity = 0;
	int playerId;

	pugi::xml_node parameters;
	void SmoothMove();
	void StartMovement();

	void BasicAnimationState();
	virtual void SpecificAnimationState();

	void ActivateDmgEasing();

protected:

	int moveTimer = 0;
	iPoint moveVector = { 0,0 };
	int speed = 4;
	const int timeForATile = 2;
	const int tileSize = 16;
	iPoint destination;

	eastl::unique_ptr<Easing> dmgEasingIn;
	eastl::unique_ptr<Easing> dmgEasingOut;
	float sillyDmg;
	eastl::unique_ptr<Easing> sillyEasingJump;
	eastl::unique_ptr<Easing> sillyEasingFall;
	float sillyJump;
	ActionState state;
	bool facing;
	Animation* currentAnim = nullptr;
	Animation idleLeftAnim;
	Animation idleRightAnim;

	SDL_Texture* texture;
	SString name;
	UnitType type;

	const char* texturePath;

	bool isMyTurn = false;
	bool hasFinishedTurn = false;
	int healthPoints = 1;
	int damage;
	int maxHealth;
};

#endif //__PLAYER_H__
