#include "CollectibleObject.h"
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

CollectibleObject::CollectibleObject(bool solved) : Entity(EntityType::COLLECTIBLEOBJECT)
{
	this->solved = solved;
	name.Create("collectibleObject");
}

CollectibleObject::~CollectibleObject() {}

bool CollectibleObject::Awake() {


	return true;
}

bool CollectibleObject::Start() {

	// initilize textures

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();
	texture = app->tex->Load(texturePath);

	anim.PushBack({ 0 * 16,0 * 16,24,43 });
	anim.loop = false;
	anim.speed = 0.0f;
	w = 24;
	h = 43;


	cRect = { position.x,position.y,24,43 };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::COLLECTIBLEOBJECT, (Entity*)this);

	return true;
}

bool CollectibleObject::Update()
{
	if (solved)
	{
		return true;
	}
	SDL_Rect rect = anim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	anim.Update();

	eCollider->SetPos(position.x, position.y);

	return true;
}

bool CollectibleObject::CleanUp()
{
	app->tex->Unload(texture);
	eCollider->pendingToDelete = true;
	return true;
}

void CollectibleObject::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::COLLECTIBLEOBJECT)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::PLAYER:
			//FUNCTION TO BE COLLECTED
			if (app->questManager->sidequestActive->id == COLLECT1) {
				app->questManager->ObjectsCount++;
				eCollider->type = Collider::Type::NONE;
				this->solved = true;
				this->toDelete = true;
				break;
			}
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
}