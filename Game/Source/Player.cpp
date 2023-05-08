#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"
#include "ModuleCollisions.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");

	// idle player
	//idleanim.PushBack({ 0, 0, 32, 32 });
	//idleanim.loop = false;
	//idleanim.speed = 0.0f;

}

Player::~Player() {

}

bool Player::Awake() {
	// Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	w = h = 16;
	return true;
}

bool Player::Start() {

	// initilize textures
	texture = app->tex->Load(texturePath);
	eCollider = app->moduleCollisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::PLAYER, (Entity*)this);
	currentAnim = &idleanim;
	mPosition.x = METERS_TO_PIXELS((float)position.x);
	mPosition.y = METERS_TO_PIXELS((float)position.y);
	return true;
}

bool Player::Update()
{
	int speed = 1;


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		mPosition.y -= speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		mPosition.y += speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		mPosition.x -= speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		mPosition.x += speed;
	}

	//also move collider

	position.x = PIXEL_TO_METERS((int)mPosition.x);
	position.y = PIXEL_TO_METERS((int)mPosition.y);
	eCollider->SetPos(position.x, position.y);

	//PLAYER MOVEMENT
	//if (app->scene->questMenu) {}
	//else if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)) && tile.y > 0)
	//{
	//	tile.y --;
	//}
	//else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)) && tile.y < 21)
	//{
	//	tile.y ++;
	//}
	//else if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)) && tile.x > 0)
	//{
	//	tile.x --;
	//}
	//else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)) && tile.x < 37)
	//{
	//	tile.x ++;
	//}
	
	//WINNING SEQUENCE
	
	//ANIMATION STATE MACHINE

	//SDL_Rect rect = currentAnim->GetCurrentFrame();
	//currentAnim->Update();

	return true;
}

bool Player::PostUpdate() {

	app->render->DrawTexture(texture, position.x, position.y);
	return true;
}

bool Player::CleanUp()
{
	app->tex->Unload(texture);
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::PLAYER)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			//FUNCION TO NOT TRASPASS
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
			break;
		case Collider::Type::ENEMY:
			//FUNCION TO START BATTLE
			break;
		case Collider::Type::NPC:
			//FUNCTION TO NOT TRASPASS
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
			break;
		case Collider::Type::NPCINTERACTION:
			//DIALOG FUNCTION + QUEST?
			break;
		case Collider::Type::CAMLIMIT:
			//FUNCTION TO MAKE CAMERA FOLLOW PLAYER
			break;
		case Collider::Type::CHEAST:
			//FUNCTION TO NOT TRASPASS
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
			break;
		case Collider::Type::CHEASTINTERACTION:
			//FUNCTION TO OPEN CHEAST
			break;
		case Collider::Type::CHANGESCENE:
			//FUNCTION TO CHANGE SCENE LOL
			break;
		default:
			break;
		}
	}

}

