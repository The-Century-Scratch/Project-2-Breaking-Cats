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
enum Facing
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};
enum class UnitType
{
	UNDEFINED,
	GATS,
	CATSKA,	
	GUARDIAN,
	LONGRANGE,
	STRAW,
	SERPICAT
};

class Unit : public Transform
{
public:
	struct PlayerAction
	{
		enum class Action
		{
			NONE,
			MOVE,
			INTERACT,
			ATTACK,
			ATTACK_LONG_RANGE,
			ATTACK_TO_PLAYER,
			ATTACK_TO_PLAYER_LONG_RANGE,
			PREPARE_DASH,
			ATTACK_AND_HEAL_WITH_KILL,
			GRENADE,
			TELEPORT,
			PORTAL,
			SILLYMAGIC
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
	void Heal(int amount);
	int GetHealthPoints();
	int GetDamage();
	int GetMagic();
	int GetPlayerId();
	SString GetName();
	UnitType GetType();
	bool GetIsAlly();
	bool GetIsMelee();

	void Update();

	int velocity = 0;
	int playerId;
	Facing gridFacing;

	pugi::xml_node parameters;
	void SmoothMove();
	void StartMovement();

	void BasicAnimationState();
	virtual void SpecificAnimationState();

	void ActivateDmgEasing();

	void ActivateAtkEasing();

	bool atkEasingHasEnded();

protected:

	int moveTimer = 0;
	iPoint moveVector = { 0,0 };
	int speed = 4;
	const int timeForATile = 2;
	const int tileSize = 16;
	iPoint destination;

	eastl::unique_ptr<Easing> atkEasingIn;
	eastl::unique_ptr<Easing> atkEasingOut;
	float sillyAtk = 0;
	eastl::unique_ptr<Easing> dmgEasingIn;
	eastl::unique_ptr<Easing> dmgEasingOut;
	float sillyDmg = 0;
	eastl::unique_ptr<Easing> sillyEasingJump;
	eastl::unique_ptr<Easing> sillyEasingFall;
	float sillyJump = 0;
	ActionState state;
	bool facing = 0;
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
	int damage = 0;
	int magic = 0;
	int maxHealth = 0;
};

#endif //__PLAYER_H__
