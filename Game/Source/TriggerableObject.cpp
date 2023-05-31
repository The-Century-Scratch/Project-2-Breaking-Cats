#include "TriggerableObject.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "SceneManager.h"
#include "Hud.h"

TriggerableObject::TriggerableObject(bool triggered) : Entity(EntityType::TRIGGERABLEOBJECT)
{
	this->triggered = triggered;
	name.Create("triggerableObject");
}

TriggerableObject::~TriggerableObject() {}

bool TriggerableObject::Awake() {


	return true;
}

bool TriggerableObject::Start() {

	// initilize textures

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();

	texture = app->tex->Load(texturePath);

	untriggeredAnim.PushBack({ 0 * 16,0 * 16,16,16 });
	untriggeredAnim.loop = false;
	untriggeredAnim.speed = 0.0f;

	triggeredAnim.PushBack({ 1 * 16,0 * 16,16,16 });
	triggeredAnim.loop = false;
	triggeredAnim.speed = 0.0f;

	w = h = 16;

	cRect = { position.x - 6,position.y - 6,16 + 12,16 + 12 };

	boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::TRIGGERABLEOBJECT, (Entity*)this);
	cRect = { position.x,position.y,16,16 };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::WALL, (Entity*)this);

	return true;
}

bool TriggerableObject::Update()
{
	if (inRange)
	{
		//keyboard input
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			app->sceneManager->puzzle2solved = true;
			if (triggered)
			{
				currentAnim = &untriggeredAnim;
				app->audio->PlayFx(app->hud->unswitchfx);
				triggered = false;
			}
			else if (!triggered)
			{
				currentAnim = &triggeredAnim;
				app->audio->PlayFx(app->hud->switchfx);
				triggered = true;
			}
			SDL_ShowCursor(SDL_ENABLE);
		}
		//controller input
		else if (CONTROLLERA)
		{
			app->sceneManager->puzzle2solved = true;
			if (triggered)
			{
				currentAnim = &untriggeredAnim;
				app->audio->PlayFx(app->hud->unswitchfx);
				triggered = false;
			}
			else if (!triggered)
			{
				currentAnim = &triggeredAnim;
				app->audio->PlayFx(app->hud->switchfx);
				triggered = true;
			}
			SDL_ShowCursor(SDL_DISABLE);
		}

	}
	if (triggered)
	{
		currentAnim = &triggeredAnim;
	}
	else if (!triggered)
	{
		currentAnim = &untriggeredAnim;
	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	currentAnim->Update();
	return true;
}

bool TriggerableObject::CleanUp()
{
	app->tex->Unload(texture);
	eCollider->pendingToDelete = true;
	boundaries->pendingToDelete = true;
	return true;
}

void TriggerableObject::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::WALL)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			break;
		case Collider::Type::ENEMY:
			break;
		case Collider::Type::NPCINTERACTION:
			break;
		case Collider::Type::CAMLIMIT:
			break;
		case Collider::Type::CHEAST:
			break;
		case Collider::Type::CHEASTINTERACTION:
			break;
		case Collider::Type::CHANGESCENE:
			break;
		default:
			break;
		}
	}
	else if (c1->type == Collider::Type::TRIGGERABLEOBJECT)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			inRange = true;
			break;
		case Collider::Type::ENEMY:
			break;
		case Collider::Type::NPC:
			break;
		case Collider::Type::CAMLIMIT:
			break;
		case Collider::Type::CHEAST:
			break;
		case Collider::Type::CHEASTINTERACTION:
			break;
		case Collider::Type::CHANGESCENE:
			break;
		default:
			break;
		}
	}
}

void TriggerableObject::EndCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::WALL)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			break;
		case Collider::Type::ENEMY:
			break;
		case Collider::Type::NPCINTERACTION:
			break;
		case Collider::Type::CAMLIMIT:
			break;
		case Collider::Type::CHEAST:
			break;
		case Collider::Type::CHEASTINTERACTION:
			break;
		case Collider::Type::CHANGESCENE:
			break;
		default:
			break;
		}
	}
	else if (c1->type == Collider::Type::TRIGGERABLEOBJECT)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			inRange = false;
			break;
		case Collider::Type::ENEMY:
			break;
		case Collider::Type::NPC:
			break;
		case Collider::Type::CAMLIMIT:
			break;
		case Collider::Type::CHEAST:
			break;
		case Collider::Type::CHEASTINTERACTION:
			break;
		case Collider::Type::CHANGESCENE:
			break;
		default:
			break;
		}
	}
}