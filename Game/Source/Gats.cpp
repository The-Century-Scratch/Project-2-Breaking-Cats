#include "App.h"
#include "Gats.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"

Gats::Gats() = default;

Gats::~Gats() = default;


void Gats::DebugDraw() const
{
	
	int intensity = 255.0f * (static_cast<float>(healthPoints) / 50);
	
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	if (isMyTurn)
	{
		app->render->DrawShape(debugPosition, false, SDL_Color(intensity, intensity, 0, 255));
	}
	else
	{
		app->render->DrawShape(debugPosition, false, SDL_Color(intensity, 0, intensity, 255));
	}
	debugPosition.y += 13;
	debugPosition.h /= 3;
	debugPosition.w = intensity / 12;


	app->render->DrawShape(debugPosition, true, SDL_Color(255 - intensity, intensity, 0, 255));
}

void Gats::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
}

bool Gats::GetIsMyTurn()
{
	return isMyTurn;
}

bool Gats::GetHasFinishedTurn()
{
	return hasFinishedTurn;
}

void Gats::SetIsMyTurn(bool value)
{
	isMyTurn = value;
	//return isMyTurn;
}

void Gats::SetHasFinishedTurn(bool value)
{
	hasFinishedTurn = value;
	//return hasFinishedTurn;
}

void Gats::Create(iPoint pos)
{
	/*Sprite::Initialize("Assets/Maps/Slime.png", 4);
	position = { 48, 272 };
	size = { 48, 48 };*/
	texture = app->tex->Load("Assets/Maps/GatsIdle.png");
	//Sprite::Initialize("Assets/Maps/GatsIdle.png", 4);
	position = pos;
	size = { 16, 16 };
	/*currentSpriteSlice = {
		(GetTextureIndex().x + 1) * size.x,
		GetTextureIndex().y * size.y,
		size.x,
		size.y
	};*/
}

Gats::PlayerAction Gats::HandleInput() const
{
	using enum KeyState;
	using enum Gats::PlayerAction::Action;

	PlayerAction returnAction = { position, NONE };

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		returnAction.action = ATTACK;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.x += tileSize;
	}

	return returnAction;
}

void Gats::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{
		//LOG("according to the software the character is moving right now......");
		StartMovement();
	}
	else if (playerAction.action == PlayerAction::Action::ATTACK)
	{
		
	}
}

void Gats::StartMovement()
{
	using enum KeyState;
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
}

void Gats::Update()
{
	//LOG("the move vector x is %i" moveVector.x);

	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();

	}
	//hasFinishedTurn = true;

	//moveTimer = 2;
}

void Gats::AnimateMove()
{
	if (animTimer == 8)
	{
		currentSpriteSlice.x += size.x;
		if (currentSpriteSlice.x == size.x * (GetTextureIndex().x + 3))
		{
			currentSpriteSlice.x = GetTextureIndex().x * size.x;
		}
		animTimer = 0;
	}
	else
	{
		animTimer++;
	}
}

void Gats::SmoothMove()
{


	if (moveTimer == timeForATile)
	{


		moveTimer = 0;
		position += (moveVector * speed);
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

bool Gats::GetIsAlly()
{
	return true;
}

void Gats::DealDamage(int amount)
{
	healthPoints -= amount;
}

int Gats::GetHealthPoints()
{
	return healthPoints;
}