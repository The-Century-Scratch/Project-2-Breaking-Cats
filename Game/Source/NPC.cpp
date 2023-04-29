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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	npcid = parameters.attribute("id").as_int();
	int character = parameters.attribute("character").as_int();

	NPCAnim.PushBack({ (1 + character) * 32,0 * 32,32,32 });
	NPCAnim.loop = false;
	NPCAnim.speed = 0.0f;

	cRect = { position.x - 6,position.y - 6,32 + 12,30 + 12 };
	boundaries = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPCINTERACTION, (Entity*)this);
	cRect = { position.x,position.y,32,32 };
	cNpc = app->moduleCollisions->AddCollider(cRect, Collider::Type::NPC, (Entity*)this);


	return true;
}

bool NPC::Start() {

	// initilize textures
	texture = app->tex->Load(texturePath);

	return true;
}

bool NPC::Update()
{
	SDL_Rect rect = NPCAnim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	NPCAnim.Update();

	return true;
}

bool NPC::CleanUp()
{
	return true;
}

void NPC::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::NPC)
	{
		
	}
}