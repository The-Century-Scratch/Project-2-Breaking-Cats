#include "App.h"
#include "Unit.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"

Unit::Unit() = default;

Unit::~Unit() = default;


void Unit::DebugDraw() const
{
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	app->render->DrawShape(debugPosition, false, SDL_Color(255, 0, 0, 255));
}

void Unit::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
}

void Unit::Create(iPoint pos)
{
	/*Sprite::Initialize("Assets/Maps/Slime.png", 4);
	position = { 48, 272 };
	size = { 48, 48 };*/
	texture = app->tex->Load("Assets/Maps/TheGuardian.png");
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

Unit::PlayerAction Unit::HandleInput() const
{
	using enum KeyState;
	using enum Unit::PlayerAction::Action;

	PlayerAction returnAction = { position, NONE };

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
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
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		moveVector.y = -1;
		//currentSpriteSlice.y = (GetTextureIndex().y + 3) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		moveVector.x = -1;
		//currentSpriteSlice.y = (GetTextureIndex().y + 1) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		moveVector.y = 1;
		//currentSpriteSlice.y = GetTextureIndex().y * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		moveVector.x = 1;
		LOG("it does enter this scope %i", moveVector.x);
		//currentSpriteSlice.y = (GetTextureIndex().y + 2) * size.y;
	}
}

void Unit::Update()
{
	
	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();
		LOG("it does enter this scope");
	}
}

void Unit::AnimateMove()
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

void Unit::SmoothMove()
{
	if (moveTimer == timeForATile)
	{
		moveTimer = 0;
		position += (moveVector * speed);
		if (position.x % tileSize == 0 && position.y % tileSize == 0)
		{
			moveVector.SetToZero();
		}
	}
	else
	{
		moveTimer++;
	}
}
