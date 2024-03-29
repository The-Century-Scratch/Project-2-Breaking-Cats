#include "App.h"
#include "Catska.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Inventory.h"
#include "Audio.h"
#include "Hud.h"

//Catska::Catska() = default;

Catska::Catska()
{
	name.Create("catska");
}

Catska::~Catska() = default;


void Catska::DebugDraw() const
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

void Catska::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	//app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y);
}

bool Catska::GetIsMyTurn()
{
	return isMyTurn;
}

bool Catska::GetHasFinishedTurn()
{
	return hasFinishedTurn;
}

void Catska::SetIsMyTurn(bool value)
{
	isMyTurn = value;
	//return isMyTurn;
}

void Catska::SetHasFinishedTurn(bool value)
{
	hasFinishedTurn = value;
	//return hasFinishedTurn;
}

void Catska::Create(iPoint pos)
{
	
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath); // TODO: find a way to use texturePath instead of hardcoding it
	
	position = pos;
	size = { 16, 16 };

	if (app->inventory->GetMysticalEnergy())
	{
		damage += 5;
	}
	
}

Catska::PlayerAction Catska::HandleInput() const
{
	//using enum KeyState;
	//using enum Catska::PlayerAction::Action;

	PlayerAction returnAction = { position, Catska::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		returnAction.action = Catska::PlayerAction::Action::ATTACK_LONG_RANGE;
		app->audio->PlayFx(app->hud->attkcatskafx);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		returnAction.action |= Catska::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		returnAction.action |= Catska::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x -= tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		returnAction.action |= Catska::PlayerAction::Action::MOVE;
		returnAction.destinationTile.y += tileSize;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		returnAction.action |= Catska::PlayerAction::Action::MOVE;
		returnAction.destinationTile.x += tileSize;
	}

	return returnAction;
}

void Catska::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		StartMovement();
	}
	LOG("it does enter this scope right now, so be careful");
}

void Catska::StartMovement()
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
}

void Catska::Update()
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

void Catska::AnimateMove()
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

bool Catska::GetIsAlly()
{
	return true;
}

void Catska::SmoothMove()
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

void Catska::DealDamage(int amount)
{
	if (app->inventory->GetGrapplingHook())
	{
		int aux = rand() % 4;
		if (aux > 0)
		{
			healthPoints -= amount;
			LOG("damage avoided");
			app->audio->PlayFx(app->hud->dmgcatskafx);
		}
	}
	else
	{
		app->audio->PlayFx(app->hud->dmgcatskafx);
		healthPoints -= amount;
	}
	
}

int Catska::GetHealthPoints()
{
	return healthPoints;
}

int Catska::GetDamage()
{
	return damage;
}
