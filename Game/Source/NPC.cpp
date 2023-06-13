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
		NPCIdle.PushBack({ 0,0,32,32 });
		NPCIdle.PushBack({ 32,0,32,32 });
		NPCIdle.PushBack({ 64,0,32,32 });
		NPCIdle.PushBack({ 32,0,32,32 });
		NPCIdle.PushBack({ 64,0,32,32 });
		NPCIdle.loop = true;
		NPCIdle.speed = 0.1f;
		//anim2
		NPCIdleAction.speed = -1;
		break;
	case NPCTYPE::VILLAGE:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 1;
		//anim
		NPCIdle.PushBack({ 0,0,32,32 });
		NPCIdle.loop = false;
		NPCIdle.speed = 0.0f;
		//anim2
		NPCIdleAction.speed = -1;
		break;
	case NPCTYPE::GUARDIAN:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 2;
		//anim1
		for (int i = 0; i < 8; ++i) {
			NPCIdle.PushBack({ 32 * i,0,32,32 });
		}
		NPCIdle.loop = true;
		NPCIdle.speed = 0.1f;
		//anim2
		for (int i = 8; i < 15; ++i) {
			NPCIdleAction.PushBack({ 32 * i,0,32,32 });
		}
		NPCIdleAction.loop = false;
		NPCIdleAction.speed = 0.1f;
		break;
	case NPCTYPE::SHOP: //shop boundries must be lowered in y position
		//collider
		cRect = { position.x - 6,position.y + 27,16 + 12,16 + 12 };
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 1;
		//anim1
		for (int i = 0; i < 7; ++i) {
			NPCIdle.PushBack({ 32 * i,0,32,32 });
		}
		NPCIdle.loop = true;
		NPCIdle.speed = 0.1f;
		//anim2
		for (int i = 7; i < 17; ++i) {
			NPCIdleAction.PushBack({ 32 * i,0,32,32 });
		}
		NPCIdleAction.loop = false;
		NPCIdleAction.speed = 0.2f;
		break;
	case NPCTYPE::CONTRABANDIST:
		//collider
		boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
		cRect = { position.x + 8,position.y + 16,16,16 };
		eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);
		//dialogue id
		dialogueid = 3;
		//anim1
		for (int i = 0; i < 5; ++i) {
			NPCIdle.PushBack({ 32*i,0,32,32 });
		}
		NPCIdle.loop = true;
		NPCIdle.speed = 0.1f;
		//anim2
		for (int i = 5; i < 24; ++i) {
			NPCIdleAction.PushBack({ 32*i,0,32,32 });
		}
		NPCIdleAction.loop = false;
		NPCIdleAction.speed = 0.1f;
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
	//anim things basically trigger the action for the anim if the npc has it check if speed is -1 to know if the action for the idle animation exists or no
	if (this->NPCIdleAction.speed == -1) {
		SDL_Rect rect = NPCIdle.GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		NPCIdle.Update();
	}
	else {
		++actionanimcounter;
		if (actionanimcounter >= 400) {
			SDL_Rect rect = NPCIdleAction.GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
			NPCIdleAction.Update();
			if (NPCIdleAction.HasFinished() == true) {
				actionanimcounter = 0;
				NPCIdleAction.Reset();
			}
		}
		else {
			SDL_Rect rect = NPCIdle.GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
			NPCIdle.Update();
		}
	}
	
	return true;
}

bool NPC::PostUpdate()
{
//anim things basically trigger the action for the anim if the npc has it check if speed is -1 to know if the action for the idle animation exists or no
	if (this->NPCIdleAction.speed == -1) {
		SDL_Rect rect = NPCIdle.GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		NPCIdle.Update();
	}
	else {
		++actionanimcounter;
		if (actionanimcounter >= 400) {
			SDL_Rect rect = NPCIdleAction.GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
			NPCIdleAction.Update();
			if (NPCIdleAction.HasFinished() == true) {
				actionanimcounter = 0;
				NPCIdleAction.Reset();
			}
		}
		else {
			SDL_Rect rect = NPCIdle.GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect);
			NPCIdle.Update();
		}
	}
	
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