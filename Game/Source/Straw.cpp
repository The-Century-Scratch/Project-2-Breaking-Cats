#include "App.h"
#include "Straw.h"

#include "Map.h"
#include "Log.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"

Straw::Straw() = default;

Straw::~Straw() = default;

void Straw::Create(iPoint pos)
{
	texturePath = parameters.attribute("value").as_string();
	texture = app->tex->Load(texturePath);

	healthPoints = 30;
	damage = 666;

	position = pos;
	size = { 16, 16 };
	type = UnitType::STRAW;
}


void Straw::DebugDraw() const
{
	int intensity = 255.0f * (static_cast<float>(healthPoints) / 30);
	
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

void Straw::Draw() const
{
	iPoint Displacement = { 8,24 };
	DebugDraw();
	//app->render->DrawTexture(DrawParameters(/*GetTextureID()*/texture, position - Displacement)/*.Section(&currentSpriteSlice)*/);
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y);
}

Straw::PlayerAction Straw::HandleInput() const
{
	//using enum KeyState;
	//using enum Straw::PlayerAction::Action;

	PlayerAction returnAction = { position, Straw::PlayerAction::Action::NONE };

	if (!moveVector.IsZero())
		return returnAction;

	return returnAction;
}

void Straw::StartAction(PlayerAction playerAction)
{
	if (playerAction.action == PlayerAction::Action::MOVE)
	{

		StartMovement();
	}
}