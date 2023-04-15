#include "App.h"
#include "Guardian.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"

Guardian::Guardian() = default;

Guardian::~Guardian() = default;


void Guardian::DebugDraw() const
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

void Guardian::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
}

bool Guardian::GetIsMyTurn()
{
	return isMyTurn;
}

bool Guardian::GetHasFinishedTurn()
{
	return hasFinishedTurn;
}

void Guardian::SetIsMyTurn(bool value)
{
	isMyTurn = value;
	//return isMyTurn;
}

void Guardian::SetHasFinishedTurn(bool value)
{
	hasFinishedTurn = value;
	//return hasFinishedTurn;
}

void Guardian::Create(iPoint pos)
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

Guardian::PlayerAction Guardian::HandleInput() const
{
	using enum KeyState;
	using enum Guardian::PlayerAction::Action;

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

void Guardian::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		StartMovement();
	}
}

void Guardian::StartMovement()
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

void Guardian::Update()
{
	//LOG("the move vector x is %i" moveVector.x);

	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();

	}

	//isMyTurn = false;
	hasFinishedTurn = true;
	//moveTimer = 2;
}

void Guardian::AnimateMove()
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

void Guardian::SmoothMove()
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
