#include "App.h"
#include "Serpicat.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Inventory.h"
#include "Audio.h"
#include "Hud.h"

//Catska::Catska() = default;

Serpicat::Serpicat()
{
	name.Create("serpicat");

	texturePath = "Assets/Textures/Serpicat.png";
	healthPoints = 40;
	damage = 3;
	magic = 5;
	type = UnitType::SERPICAT;

	idleLeftAnim.AnimateCat32x32(5, 0);
	idleLeftAnim.speed = 0.2f;

	idleRightAnim.AnimateCat32x32(5, 1);
	idleRightAnim.speed = 0.2f;

	currentAnim = &idleLeftAnim;
	state = ActionState::IDLE;
	facing = FACING_LEFT;
}

Serpicat::~Serpicat() = default;


void Serpicat::DebugDraw() const
{
	
	int intensity = 255.0f * (static_cast<float>(healthPoints) / 40);
	
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

Serpicat::PlayerAction Serpicat::HandleInput() const
{
	//using enum KeyState;
	//using enum Catska::PlayerAction::Action;

	PlayerAction returnAction = { position, Serpicat::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		returnAction.action = Serpicat::PlayerAction::Action::SILLYMAGIC;
	}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		returnAction.action = Serpicat::PlayerAction::Action::PORTAL;
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		returnAction.action = Serpicat::PlayerAction::Action::TELEPORT;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		returnAction.action |= Serpicat::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		returnAction.action |= Serpicat::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		returnAction.action |= Serpicat::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		returnAction.action |= Serpicat::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x += tileSize;
	}

	return returnAction;
}

void Serpicat::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		destination = playerAction.destinationTile;
		StartMovement();
	}
	else if (playerAction.action == PlayerAction::Action::TELEPORT)
	{
		position = playerAction.destinationTile;
	}
	LOG("it does enter this scope right now, so be careful");
}
