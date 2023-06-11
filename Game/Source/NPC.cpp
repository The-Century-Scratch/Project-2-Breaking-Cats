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
#include "QuestManager.h"

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
	case NPCTYPE::SIGN:
		//collider
		cRect = { position.x - 6,position.y - 6,32 + 12,32 + 12 };
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 6,position.y + 4,20,22 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 0;
		//anim
		NPCAnim.PushBack({ 0,0,32,32 });
		NPCAnim.PushBack({ 32,0,32,32 });
		NPCAnim.PushBack({ 64,0,32,32 });
		NPCAnim.PushBack({ 32,0,32,32 });
		NPCAnim.PushBack({ 64,0,32,32 });
		NPCAnim.loop = true;
		NPCAnim.speed = 0.1f;
		break;
	case NPCTYPE::VILLAGE:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 1;
		break;
	case NPCTYPE::GUARDIAN:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id

		dialogueid = 2;
		break;
	case NPCTYPE::SHOP: //shop boundries must be lowered in y position
		//collider
		cRect = { position.x - 6,position.y + 27,16 + 12,16 + 12 };
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 1;
		break;
	case NPCTYPE::CONTRABANDIST:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 3;
		break;
	
	default:
		break;
	}

	return true;
}

bool NPC::Update()
{

	switch (npctype)
	{
	case NPCTYPE::GUARDIAN:
		break;
	case NPCTYPE::VILLAGE:
		if (app->questManager->changeDialogueIdAfterRocks) {
			this->dialogueid = 5;
			app->questManager->changeDialogueIdAfterRocks = false;
		}
		break;
	case NPCTYPE::SHOP:
		break;
	case NPCTYPE::CONTRABANDIST:
		if (app->questManager->changeDialogueIdAfterCollecting) {
			this->dialogueid = 4;
			app->questManager->changeDialogueIdAfterCollecting = false;
		}
		break;
	case NPCTYPE::SIGN:
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
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				LOG("TRIGGER DIALOGUE");
				app->sceneManager->dialogueManager->LoadDialogue(this->dialogueid);
				app->sceneManager->dialogueManager->printText = true;
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