#include "App.h"
#include "Straw.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"

Straw::Straw() = default;

Straw::~Straw() = default;


void Straw::DebugDraw() const
{
	
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	if (isMyTurn)
	{
		app->render->DrawShape(debugPosition, false, SDL_Color(255, 255, 0, 255));
	}
	else
	{
		app->render->DrawShape(debugPosition, false, SDL_Color(255, 0, 255, 255));
	}
	
}

void Straw::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
}

bool Straw::GetIsMyTurn()
{
	return isMyTurn;
}

bool Straw::GetHasFinishedTurn()
{
	return hasFinishedTurn;
}

void Straw::SetIsMyTurn(bool value)
{
	isMyTurn = value;
	//return isMyTurn;
}

void Straw::SetHasFinishedTurn(bool value)
{
	hasFinishedTurn = value;
	//return hasFinishedTurn;
}

void Straw::Create(iPoint pos)
{
	/*Sprite::Initialize("Assets/Maps/Slime.png", 4);
	position = { 48, 272 };
	size = { 48, 48 };*/
	texture = app->tex->Load("Assets/Maps/StrawDall.png");
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

Straw::PlayerAction Straw::HandleInput() const
{
	using enum KeyState;
	using enum Straw::PlayerAction::Action;

	PlayerAction returnAction = { position, NONE };

	if (!moveVector.IsZero())
		return returnAction;

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

void Straw::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		StartMovement();
	}
}

void Straw::StartMovement()
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
		//LOG("it does enter this scope %i", moveVector.x);
		//currentSpriteSlice.y = (GetTextureIndex().y + 2) * size.y;
	}
}

void Straw::Update()
{
	//LOG("the move vector x is %i" moveVector.x);
	LOG("This is definitely completely a different update funnction to the other update functions that you have seen so far, so trust me, it is good");

	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();

	}


	//moveTimer = 2;
}

void Straw::AnimateMove()
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

void Straw::SmoothMove()
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
	LOG("it does enter this scope %i", moveTimer);
}
