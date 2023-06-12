#include "App.h"
#include "Gats.h"

#include "Map.h"
#include "Log.h"
#include "Animation.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Inventory.h"
#include "Audio.h"
#include "Hud.h"

//Gats::Gats() = default;
Gats::Gats()
{
	name.Create("gats");

	healthPoints = 50;
	damage = 10;
	type = UnitType::GATS;

	if (app->inventory->GetArcaneSpirit())
	{
		healthPoints += 15;
	}

	idleLeftAnim.AnimateCat32x32(5, 6);
	idleLeftAnim.speed = 0.2f;

	idleRightAnim.AnimateCat32x32(5, 7);
	idleRightAnim.speed = 0.2f;

	currentAnim = &idleLeftAnim;
	state = ActionState::IDLE;
	facing = FACING_LEFT;
}

Gats::~Gats() = default;


void Gats::DebugDraw() const
{
	
	int intensity = 255.0f * (static_cast<float>(healthPoints) / maxHealth);
	
	SDL_Rect debugPosition = { position.x, position.y, size.x, size.y };
	if (isMyTurn)
	{
		//app->render->DrawShape(debugPosition, false, SDL_Color(intensity, intensity, 0, 255));
		app->render->DrawRectangle(debugPosition, intensity, intensity, 255 * goingToDash, 255, false);
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

void Gats::Test()
{
	LOG("the test has been successful");
}

Gats::PlayerAction Gats::HandleInput() const
{
	PlayerAction returnAction = { position, Gats::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (app->inventory->GetDragonSlayer())
		{
			returnAction.action = Gats::PlayerAction::Action::ATTACK_AND_HEAL_WITH_KILL;
		}
		else
		{
			returnAction.action = Gats::PlayerAction::Action::ATTACK;
		}

		app->audio->PlayFx(app->hud->attkgatsfx);
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		returnAction.action = Gats::PlayerAction::Action::TELEPORT;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		returnAction.action |= Gats::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		returnAction.action |= Gats::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		returnAction.action |= Gats::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		returnAction.action |= Gats::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x += tileSize;
	}
	else if (goingToDash && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{

	}
	else if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN/* && app->inventory->GetFirePaw()*/)
	{
		returnAction.action |= Gats::PlayerAction::Action::PREPARE_DASH;
	}

	return returnAction;
}

void Gats::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{
		//LOG("according to the software the character is moving right now......");
		destination = playerAction.destinationTile;
		StartMovement();
	}
	else if (playerAction.action == PlayerAction::Action::ATTACK)
	{
		//LOG("this is a normal attack");
	}
	else if (playerAction.action == PlayerAction::Action::ATTACK_AND_HEAL_WITH_KILL)
	{
		healthPoints += 10;
		if (healthPoints > maxHealth)
		{
			healthPoints = maxHealth;
		}
		//LOG("this is a healing attack");
	}
	else if (playerAction.action == PlayerAction::Action::PREPARE_DASH)
	{
		destination = playerAction.destinationTile;
		moveVector = {(playerAction.destinationTile.x - position.x) / tileSize, (playerAction.destinationTile.y - position.y) / tileSize};
	}
	else if (playerAction.action == PlayerAction::Action::TELEPORT)
	{
		position = playerAction.destinationTile;
	}


	if (moveVector.x > 0)
		facing = FACING_RIGHT;
	else if (moveVector.x < 0)
		facing = FACING_LEFT;
}