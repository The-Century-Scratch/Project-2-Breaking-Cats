#include "MovableObject.h"
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

MovableObject::MovableObject(bool solved) : Entity(EntityType::MOVABLEOBJECT)
{
	this->solved = solved;
	name.Create("movableObject");
}

MovableObject::~MovableObject() {}

bool MovableObject::Awake() {


	return true;
}

bool MovableObject::Start() {

	// initilize textures

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();
	solvedPosition.x = parameters.attribute("sx").as_int();
	solvedPosition.y = parameters.attribute("sy").as_int();
	texture = app->tex->Load(texturePath);

	anim.PushBack({ 0 * 16,0 * 16,24,43 });
	anim.loop = false;
	anim.speed = 0.0f;
	w = 24;
	h = 43;


	cRect = { position.x,position.y,24,43 };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::MOVABLEOBJECT, (Entity*)this);

	return true;
}

bool MovableObject::Update()
{
	if (solved)
	{
		position = solvedPosition;
	}
	SDL_Rect rect = anim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	anim.Update();

	eCollider->SetPos(position.x, position.y);

	return true;
}

bool MovableObject::CleanUp()
{
	app->tex->Unload(texture);
	eCollider->pendingToDelete = true;
	return true;
}

void MovableObject::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::MOVABLEOBJECT)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			//FUNCTION TO NOT TRASPASS
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
			eCollider->type = Collider::Type::WALL;
			break;
		case Collider::Type::PLAYER:
			//FUNCTION TO BE PUSHED AWAY
			if (app->questManager->questActive == nullptr) {
				app->moduleCollisions->collision_solver(c1->listener, c2->rect);
				eCollider->type = Collider::Type::WALL;
				break;
			}
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
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