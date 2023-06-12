#include "App.h"
#include "LongRange.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Hud.h"

LongRange::LongRange()
{
	name.Create("unit");

	texturePath = "Assets/Textures/LargeRangeEnemy.png";
	healthPoints = 15;
	damage = 7;
	type = UnitType::LONGRANGE;

	idleLeftAnim.AnimateCat32x32(1, 0);
	idleLeftAnim.speed = 0.2f;

	currentAnim = &idleLeftAnim;
	state = ActionState::IDLE;
	facing = FACING_LEFT;
}

LongRange::~LongRange() = default;


void LongRange::DebugDraw() const
{
	
	int intensity = 255.0f * (static_cast<float>(healthPoints) / 15);
	
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	if (isMyTurn)
	{
		//app->render->DrawShape(debugPosition, false, SDL_Color(intensity, intensity, 0, 255));
		app->render->DrawRectangle(debugPosition, intensity, intensity, 0, 255, false);
	}
	else
	{
		//app->render->DrawShape(debugPosition, false, SDL_Color(intensity, 0, intensity, 255));
		app->render->DrawRectangle(debugPosition, intensity, 0, intensity, 255, false);
	}
	debugPosition.y += 13;
	debugPosition.h /= 3;
	debugPosition.w = intensity / 12;


	//app->render->DrawShape(debugPosition, true, SDL_Color(255 - intensity, intensity, 0, 255));
	app->render->DrawRectangle(debugPosition, 255 - intensity, intensity, 0, 255, true);
	
}

LongRange::PlayerAction LongRange::HandleInput() const
{
	//using enum KeyState;
	//using enum LongRange::PlayerAction::Action;

	PlayerAction returnAction = { position, LongRange::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;

	int direction = rand() % 10;

	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	//{
	//	returnAction.action |= MOVE;
	//	returnAction.destinationTile.y -= tileSize;
	//}
	//else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	//{
	//	returnAction.action |= MOVE;
	//	returnAction.destinationTile.x -= tileSize;
	//}
	//else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	//{
	//	returnAction.action |= MOVE;
	//	returnAction.destinationTile.y += tileSize;
	//}
	//else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	//{
	//	returnAction.action |= MOVE;
	//	returnAction.destinationTile.x += tileSize;
	//}

	if (direction == 0)
	{
		returnAction.action |= LongRange::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.y -= tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 1)
	{
		returnAction.action |= LongRange::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.x -= tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 2)
	{
		returnAction.action |= LongRange::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.y += tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 3)
	{
		returnAction.action |= LongRange::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.x += tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else
	{
		returnAction.action |= LongRange::PlayerAction::Action::ATTACK_TO_PLAYER_LONG_RANGE;
		app->audio->PlayFx(app->hud->attkenemyfx);
	}

	return returnAction;
}

void LongRange::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{
		destination = playerAction.destinationTile;
		if (playerAction.destinationTile.y < position.y)
		{
			moveVector.y = -1;
		}
		else if (playerAction.destinationTile.x < position.x)
		{
			moveVector.x = -1;
		}
		else if (playerAction.destinationTile.y > position.y)
		{
			moveVector.y = 1;
		}
		else if (playerAction.destinationTile.x > position.x)
		{
			moveVector.x = 1;
		}
	}
}