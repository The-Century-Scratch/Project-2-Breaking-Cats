#include "StaticObject.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "SceneManager.h"
#include "EntityManager.h"

StaticObject::StaticObject( ) : Entity(EntityType::STATICOBJECT)
{
	this->triggered = triggered;
	name.Create("staticObject");
}

StaticObject::~StaticObject() {}

bool StaticObject::Awake() {


	return true;
}

bool StaticObject::Start() {

	// initilize textures

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();

	texture = app->tex->Load(texturePath);

	if (id == 1)
	{
		w = 112;
		h = 32;
		anim.PushBack({ 0 * 16,0 * 16,112,32 });
	}
	if (id == 2)
	{
		w = 96;
		h = 16;
		anim.PushBack({ 0 * 16,0 * 16,96,16 });
	}

	anim.loop = false;
	anim.speed = 0.0f;



	cRect = { position.x,position.y,w,h };
	eCollider = app->moduleCollisions->AddCollider(cRect, Collider::Type::WALL);

	return true;
}

bool StaticObject::Update()
{
	if (app->sceneManager->puzzle2solved && id == 1)
	{
		CleanUp();
		//app->entityManager->DestroyEntity((Entity*)this);
	}
	if (app->sceneManager->puzzle4solved && id == 2)
	{
		CleanUp();
		//app->entityManager->DestroyEntity((Entity*)this);
	}
	SDL_Rect rect = anim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	anim.Update();
	return true;
}

bool StaticObject::CleanUp()
{
	app->tex->Unload(texture);
	eCollider->pendingToDelete = true;
	return true;
}
