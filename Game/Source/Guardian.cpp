#include "App.h"
#include "Guardian.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Hud.h"

Guardian::Guardian() = default;

Guardian::~Guardian() = default;

void Guardian::Create(iPoint pos)
{
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);

	healthPoints = 20;
	damage = 10;
	position = pos;
	size = { 16, 16 };
	type = UnitType::GUARDIAN;
}


void Guardian::DebugDraw() const
{
	
	int intensity = 255.0f * (static_cast<float>(healthPoints) / 20);
	
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

void Guardian::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	//app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y);
}

Guardian::PlayerAction Guardian::HandleInput() const
{
	//using enum KeyState;
	//using enum Guardian::PlayerAction::Action;

	PlayerAction returnAction = { position, Guardian::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;
	int direction = rand() % 19;

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
		returnAction.action |= Guardian::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.y -= tileSize;
		
		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 1)
	{
		returnAction.action |= Guardian::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.x -= tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 2)
	{
		returnAction.action |= Guardian::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.y += tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else if (direction == 3)
	{
		returnAction.action |= Guardian::PlayerAction::Action::MOVE;          // CHANGED
		returnAction.destinationTile.x += tileSize;

		//LOG("this scope is definitely entered by the movement action inside of this uncooperative template %i", returnAction.destinationTile.y);
	}
	else
	{
		returnAction.action |= Guardian::PlayerAction::Action::ATTACK_TO_PLAYER;
		app->audio->PlayFx(app->hud->attkenemyfx);
	}

	


	return returnAction;
}

void Guardian::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		
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
		StartMovement();
	}
}