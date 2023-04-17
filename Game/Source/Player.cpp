#include "App.h"
#include "Player.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"

Player::Player() = default;

Player::~Player() = default;


void Player::DebugDraw() const
{
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	app->render->DrawShape(debugPosition, false, SDL_Color(255, 0, 0, 255));
}

void Player::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
}

void Player::Create()
{
	/*Sprite::Initialize("Assets/Maps/Slime.png", 4);
	position = { 48, 272 };
	size = { 48, 48 };*/
	texture = app->tex->Load("Assets/Maps/GatsIdle.png");
	//Sprite::Initialize("Assets/Maps/GatsIdle.png", 4);
	position = { 208, 192 };
	size = { 16, 16 };
	/*currentSpriteSlice = {
		(GetTextureIndex().x + 1) * size.x,
		GetTextureIndex().y * size.y,
		size.x,
		size.y
	};*/
}

Player::PlayerAction Player::HandleInput() const
{
	using enum KeyState;
	using enum Player::PlayerAction::Action;
	using enum Direction;

	PlayerAction returnAction = { position, NONE , facing};

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		returnAction.action |= INTERACT;
	}

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y -= tileSize;
		returnAction.willFace = UP;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.x -= tileSize;
		returnAction.willFace = LEFT;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.y += tileSize;
		returnAction.willFace = DOWN;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		returnAction.action |= MOVE;
		returnAction.destinationTile.x += tileSize;
		returnAction.willFace = RIGHT;
	}

	return returnAction;
}

int Player::StartAction(PlayerAction const& playerAction, EventData const& data)
{
	int ret = 0;
	if (playerAction.action == PlayerAction::Action::MOVE)
	{
		StartMovement();
		//LOG("it does enter this scope");
	}
	else if (playerAction.action == PlayerAction::Action::INTERACT)
	{
		switch (data.commonData.type)
		{
		case EventType::CHEST:
			LOG("(CHEST)");
			break;
		case EventType::TELEPORT:
			LOG("(TP)");
			position.x = data.destinationData.destination.x;
			position.y = data.destinationData.destination.y;

			
			using enum SceneType;
			switch (data.destinationData.destinationMap)
			{
			case NEWGAME:
				ret = 1;
				break;
			case CONTINUE:
				break;
			case EXIT:
				break;
			case COMBAT:
				ret = 4;
				break;
			case VERTICAL:
				ret = 5;
				break;
			case TITLESCENE:
				ret = 6;
				break;
			case SHOP:
				ret = 7;
				break;
			case TAVERN:
				ret = 8;
				break;
			case RESISTANCE:
				ret = 9;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	return ret;
}

void Player::StartMovement()
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
		//currentSpriteSlice.y = (GetTextureIndex().y + 2) * size.y;
	}
}

void Player::Update()
{
	if (!moveVector.IsZero())
	{
		//AnimateMove();
		SmoothMove();
		//LOG("it does enter this scope");
	}
}

void Player::FaceTo(Direction dir)
{
	using enum Direction;
	switch (dir)
	{
	case DOWN:
		facing = DOWN;
		break;
	case UP:
		facing = UP;
		break;
	case RIGHT:
		facing = RIGHT;
		break;
	case LEFT:
		facing = LEFT;
		break;
	default:
		break;
	}
}

void Player::AnimateMove()
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

void Player::SmoothMove()
{
	//LOG("it does enter this scope %i", moveTimer);

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
