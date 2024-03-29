#include "App.h"
#include "Gats.h"

#include "Map.h"
#include "Log.h"

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

void Gats::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	//app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y);
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

	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);
	
	position = pos;
	size = { 16, 16 };
	if (app->inventory->GetArcaneSpirit())
	{
		healthPoints += 15;
	}
	maxHealth = healthPoints;
	
}

void Gats::Test()
{
	LOG("the test has been successful");
}

Gats::PlayerAction Gats::HandleInput() const
{
	//using enum KeyState;
	//using enum Gats::PlayerAction::Action;

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
	else if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && app->inventory->GetFirePaw())
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
		goingToDash = !goingToDash;
	}
}

void Gats::StartMovement()
{
	//using enum KeyState;
	if (goingToDash)
	{
		app->audio->PlayFx(app->hud->dashgatsfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		moveVector.y = -1 * (goingToDash * 2 + 1);
		//currentSpriteSlice.y = (GetTextureIndex().y + 3) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		moveVector.x = -1 * (goingToDash * 2 + 1);
		//currentSpriteSlice.y = (GetTextureIndex().y + 1) * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		moveVector.y = 1 * (goingToDash * 2 + 1);
		//currentSpriteSlice.y = GetTextureIndex().y * size.y;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		moveVector.x = 1 * (goingToDash * 2 + 1);
		
		//currentSpriteSlice.y = (GetTextureIndex().y + 2) * size.y;
	}
	goingToDash = false;
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
	app->audio->PlayFx(app->hud->dmggatsfx);
	healthPoints -= amount;
}

int Gats::GetHealthPoints()
{
	return healthPoints;
}

int Gats::GetDamage()
{
	return damage;
}