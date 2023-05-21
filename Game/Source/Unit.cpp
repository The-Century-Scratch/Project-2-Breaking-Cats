#include "App.h"
#include "Unit.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"

//Unit::Unit() = default;

Unit::Unit()
{
	name.Create("unit");
}

Unit::~Unit() = default;


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
	//iPoint Displacement = { 0,0 };
	DebugDraw();
	//app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y);
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

void Unit::Create(iPoint pos)
{
	
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);
	
	position = pos;
	size = { 16, 16 };
	
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
		//currentSpriteSlice.y = (GetTextureIndex().y + 3) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		moveVector.x = -1;
		//currentSpriteSlice.y = (GetTextureIndex().y + 1) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		moveVector.y = 1;
		//currentSpriteSlice.y = GetTextureIndex().y * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		moveVector.x = 1;
		
		
		//currentSpriteSlice.y = (GetTextureIndex().y + 2) * size.y;
	}
	//playerAction.action = PlayerAction::Action::NONE;
}

void Unit::Update()
{
	//LOG("the move vector x is %i", moveVector.x);

	

	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();

	}

	if (type == UnitType::STRAW)
	{
		hasFinishedTurn = true;
	}

	//moveTimer = 2;
}

void Unit::SmoothMove()
{


	if (moveTimer == timeForATile)
	{


		moveTimer = 0;
		position += (moveVector * speed);
		LOG("this is the tileSize %i, then we have the pos X: %i", tileSize, position.x);
		if (position.x % tileSize == 0 && position.y % tileSize == 0)
		{
			moveVector.SetToZero();
			hasFinishedTurn = true;
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
