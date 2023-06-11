#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "SceneManager.h"
#include "Input.h"

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("NPC");
}

NPC::~NPC() {}

bool NPC::Awake() {


	return true;
}

bool NPC::Start() {

	// initilize textures

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	npcid = parameters.attribute("id").as_int();
	npctype = (NPCTYPE)parameters.attribute("type").as_int();

	texture = app->tex->Load(texturePath);

	//NPCAnim.PushBack({ ((int)npctype) * 16,0 * 16,32,32 });
	NPCAnim.PushBack({ 0,0,32,32 });
	NPCAnim.loop = false;
	NPCAnim.speed = 0.0f;
	w = h = 32;

	cRect = { position.x - 6,position.y - 6,32 + 12,32 + 12 };

	switch (npctype)
	{
	case NPCTYPE::GUARDIAN:
		dialogueid = 0;
		break;
	case NPCTYPE::SHOP: //shop boundries must be lowered in y position
		cRect = { position.x - 6,position.y + 27,16 + 12,16 + 12 };
		dialogueid = 1;
		break;
	case NPCTYPE::CONTRABANDIST:
		break;
	default:
		break;
	}

	boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
	cRect = { position.x+8,position.y+16,16,16 };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);

	return true;
}

bool NPC::Update()
{

	switch (npctype)
	{
	case NPCTYPE::GUARDIAN:
		break;
	case NPCTYPE::SHOP:
		break;
	case NPCTYPE::CONTRABANDIST:
		break;
	default:
		break;
	}

	return true;
}

bool NPC::PostUpdate()
{
	SDL_Rect rect = NPCAnim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	NPCAnim.Update();
	return true;
}

bool NPC::CleanUp()
{
	app->tex->Unload(texture);
	eCollider->pendingToDelete = true;
	boundaries->pendingToDelete = true;
	return true;
}

void NPC::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::NPC)
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
	else if (c1->type == Collider::Type::NPCINTERACTION)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			if (!app->sceneManager->Pause && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				LOG("TRIGGER DIALOGUE");
				app->sceneManager->dialogueManager->LoadDialogue(this->dialogueid);
				app->sceneManager->dialogueManager->printText = true;
				app->sceneManager->Pause = true;
				SDL_ShowCursor(SDL_ENABLE);
			}
			else if (!app->sceneManager->Pause && CONTROLLERA)
			{
				LOG("TRIGGER DIALOGUE");
				app->sceneManager->dialogueManager->LoadDialogue(this->dialogueid);
				app->sceneManager->dialogueManager->printText = true;
				app->sceneManager->Pause = true;
				SDL_ShowCursor(SDL_DISABLE);
			}
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