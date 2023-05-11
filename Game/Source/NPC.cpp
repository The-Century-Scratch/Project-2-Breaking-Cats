#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "ModuleCollisions.h"

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

	NPCAnim.PushBack({ ((int)npctype) * 16,0 * 16,16,16 });
	NPCAnim.loop = false;
	NPCAnim.speed = 0.0f;

	cRect = { position.x - 6,position.y - 6,16 + 12,16 + 12 };

	switch (npctype)
	{
	case NPCTYPE::NORMAL:
		break;
	case NPCTYPE::QUEST:
		break;
	case NPCTYPE::SHOP: //shop boundries must be lowered in y position
		cRect = { position.x - 6,position.y + 17,16 + 12,16 + 12 };
		break;
	default:
		break;
	}

	boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
	cRect = { position.x,position.y,16,16 };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);

	return true;
}

bool NPC::Update()
{

	switch (npctype)
	{
	case NPCTYPE::NORMAL:
		break;
	case NPCTYPE::QUEST:
		break;
	case NPCTYPE::SHOP:
		break;
	default:
		break;
	}


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