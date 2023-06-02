#include "App.h"
#include "Unit.h"

#include "Map.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "easings.h"

//Unit::Unit() = default;

Unit::Unit()
{
	name.Create("unit");
}

Unit::~Unit() = default;

void Unit::Create(iPoint pos)
{
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);

	position = pos;
	size = { 16, 16 };

	sillyEasingJump = eastl::make_unique<Easing>(false, 0, 20, 0, 8);
	sillyEasingFall = eastl::make_unique<Easing>(false, 0, 0, 20, 8);

	dmgEasingIn = eastl::make_unique<Easing>(false, 0, 8, 0, 2);
	dmgEasingOut = eastl::make_unique<Easing>(false, 0, 0, 8, 2);
}

void Unit::DebugDraw() const
{
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	if (isMyTurn)
	{
		//app->render->DrawShape(debugPosition, false, SDL_Color(0, 255, 0, 255));
		app->render->DrawRectangle(debugPosition, 0, 255, 0, 255, false);
	}
	else
	{
		//app->render->DrawShape(debugPosition, false, SDL_Color(255, 0, 0, 255));
		app->render->DrawRectangle(debugPosition, 255, 0, 0, 255, false);
	}
	
}

void Unit::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();

	// Gats idle combat animation has a strange sprite displacement between left and right
	if (type == UnitType::GATS)
	{
		if (facing == FACING_LEFT)
			Displacement = { 11, 24 };
		if (facing == FACING_RIGHT)
			Displacement = { 5, 24 };
	}

	app->render->DrawTexture(texture, position.x - Displacement.x + sillyDmg, position.y - Displacement.y - sillyJump, &currentAnim->GetCurrentFrame());
}

bool Unit::GetIsMyTurn()
{
	return isMyTurn;
}

bool Unit::GetHasFinishedTurn()
{
	return hasFinishedTurn;
}

void Unit::SetIsMyTurn(bool value)
{
	isMyTurn = value;
}

void Unit::SetHasFinishedTurn(bool value)
{
	hasFinishedTurn = value;
}

Unit::PlayerAction Unit::HandleInput() const
{
	//using enum KeyState;
	//using enum Unit::PlayerAction::Action;

	PlayerAction returnAction = { position, Unit::PlayerAction::Action::NONE }; // using enum only works in c++ 20, if it isn't working, try changing the version

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		returnAction.action |= Unit::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		returnAction.action |= Unit::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		returnAction.action |= Unit::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		returnAction.action |= Unit::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x += tileSize;
	}

	return returnAction;
}

void Unit::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		StartMovement();
	}
}

void Unit::StartMovement()
{
	//using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		moveVector.y = -1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		moveVector.x = -1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		moveVector.y = 1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		moveVector.x = 1;
	}
}

void Unit::BasicAnimationState()
{
	// Function for basic animations every unit has
	switch (state)
	{
	case ActionState::IDLE:
		if (facing == FACING_LEFT)
			currentAnim = &idleLeftAnim;
		else if (facing == FACING_RIGHT)
			currentAnim = &idleRightAnim;
		break;
	case ActionState::ATTACK:
		break;
	case ActionState::DIE:
		break;
	default:
		break;
	}
}

void Unit::SpecificAnimationState()
{
	// virtual Function for specific animations that changes between enemies like Gats dash f.e.
	switch (state)
	{
	default:
		break;
	}
}

void Unit::ActivateDmgEasing()
{
	dmgEasingIn->currentIteration = 0;
	dmgEasingIn->easingsActivated = true;
}

void Unit::Update()
{
	if (!moveVector.IsZero())
	{
		SmoothMove();
	}

	BasicAnimationState();
	SpecificAnimationState();
	currentAnim->Update();


	////////// EASINGS //////////

	// JUMP EASING
	int sum = moveVector.x * moveVector.x + moveVector.y * moveVector.y;

	if (!moveVector.IsZero() && sqrt(sum) == 1)
	{
		sillyEasingJump->easingsActivated = true;
	}
	else
	{
		sillyEasingJump->currentIteration = 0;
		sillyEasingJump->easingsActivated = false;
		sillyEasingFall->currentIteration = 0;
		sillyEasingFall->easingsActivated = false;
	}

	if (sillyEasingJump->easingsActivated)
	{
		sillyJump = sillyEasingJump->sineEaseOut(sillyEasingJump->currentIteration, sillyEasingJump->initialPos, sillyEasingJump->deltaPos, sillyEasingJump->totalIterations);

		if (sillyEasingJump->currentIteration < sillyEasingJump->totalIterations)
		{
			sillyEasingJump->currentIteration++;
		}
		else
		{
			sillyEasingJump->currentIteration = 0;
			sillyEasingJump->easingsActivated = false;
			sillyEasingFall->easingsActivated = true;
		}
	}

	if (sillyEasingFall->easingsActivated)
	{
		sillyJump = sillyEasingFall->sineEaseIn(sillyEasingFall->currentIteration, sillyEasingFall->initialPos, sillyEasingFall->deltaPos, sillyEasingFall->totalIterations);

		if (sillyEasingFall->currentIteration < sillyEasingFall->totalIterations)
		{
			sillyEasingFall->currentIteration++;
		}
		else
		{
			sillyEasingFall->currentIteration = 0;
			sillyEasingFall->easingsActivated = false;
		}
	}

	// DMG EASING
	if (dmgEasingIn->easingsActivated)
	{
		sillyDmg = dmgEasingIn->sineEaseOut(dmgEasingIn->currentIteration, dmgEasingIn->initialPos, dmgEasingIn->deltaPos, dmgEasingIn->totalIterations);

		if (dmgEasingIn->currentIteration < dmgEasingIn->totalIterations)
		{
			dmgEasingIn->currentIteration++;
		}
		else
		{
			dmgEasingIn->currentIteration = 0;
			dmgEasingIn->easingsActivated = false;
			dmgEasingOut->easingsActivated = true;
		}
	}

	if (dmgEasingOut->easingsActivated)
	{
		sillyDmg = dmgEasingOut->sineEaseIn(dmgEasingOut->currentIteration, dmgEasingOut->initialPos, dmgEasingOut->deltaPos, dmgEasingOut->totalIterations);

		if (dmgEasingOut->currentIteration < dmgEasingOut->totalIterations)
		{
			dmgEasingOut->currentIteration++;
		}
		else
		{
			dmgEasingOut->currentIteration = 0;
			dmgEasingOut->easingsActivated = false;
		}
	}
	else if (!dmgEasingIn->easingsActivated)
	{
		sillyDmg = 0;
	}
}

void Unit::SmoothMove()
{


	if (moveTimer == timeForATile)
	{


		moveTimer = 0;
		position += (moveVector * speed);
		if (moveVector.x < 0)
		{
			if (position.x <= destination.x)
			{
				position = destination;
				moveVector.SetToZero();
				hasFinishedTurn = true;
			}
		}
		else if (moveVector.x > 0)
		{
			if (position.x >= destination.x)
			{
				position = destination;
				moveVector.SetToZero();
				hasFinishedTurn = true;
			}
		}

		if (moveVector.y < 0)
		{
			if (position.y <= destination.y)
			{
				position = destination;
				moveVector.SetToZero();
				hasFinishedTurn = true;
			}
		}
		else if (moveVector.y > 0)
		{
			if (position.y >= destination.y)
			{
				position = destination;
				moveVector.SetToZero();
				hasFinishedTurn = true;
			}
		}
	}
	else
	{
		moveTimer++;
	}
	
}

void Unit::DealDamage(int amount)
{
	healthPoints -= amount;
}

int Unit::GetHealthPoints()
{
	return healthPoints;
}

int Unit::GetDamage()
{
	return damage;
}

int Unit::GetPlayerId()
{
	return playerId;
}

SString Unit::GetName()
{
	return name;
}

UnitType Unit::GetType()
{
	return type;
}

bool Unit::GetIsAlly()
{
	switch (type)
	{
	case UnitType::UNDEFINED:
		return false;
		break;
	case UnitType::GATS:
		return true;
		break;
	case UnitType::CATSKA:
		return true;
		break;
	case UnitType::GUARDIAN:
		return false;
		break;
	case UnitType::LONGRANGE:
		return false;
		break;
	case UnitType::STRAW:
		return false;
		break;
	default:
		break;
	}
	return false;
}
