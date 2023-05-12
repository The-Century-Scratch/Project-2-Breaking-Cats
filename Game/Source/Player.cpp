#include "Player.h"
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
#include "SceneManager.h"
#include "window.h"
#include "Map.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");

	// IDLE
	idleAnim.AnimateCat32x32x8(0);
	idleAnim.speed = 0.2f;

	// WALKING UP
	walkUpAnim.AnimateCat32x32x4(1);
	walkUpAnim.speed = 0.2f;

	// WALKING LEFT
	walkLeftAnim.AnimateCat32x32x4(2);
	walkLeftAnim.speed = 0.2f;

	// WALKING RIGHT
	walkRightAnim.AnimateCat32x32x4(3);
	walkRightAnim.speed = 0.2f;

	// WALKING DOWN
	walkDownAnim.AnimateCat32x32x4(4);
	walkDownAnim.speed = 0.2f;

	// DYING
	dieAnim.AnimateCat32x32x4(5);
	dieAnim.speed = 0.2f;

}

Player::~Player() {

}

bool Player::Awake() {
	return true;
}

bool Player::Start() {
	// Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	w = h = 16;

	// initilize textures
	texture = app->tex->Load(texturePath);
	eCollider = app->moduleCollisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::PLAYER, (Entity*)this);
	currentAnim = &idleAnim;


	switch (app->sceneManager->currentScene)
	{
	case 0:
		break;
	case 1:
		position.x = 113;
		position.y = 366;
		break;
	case 2:
		position.x = 71;
		position.y = 173;
		break;
	case 3:
		position.x = 334;
		position.y = 106;
		break;
	default:
		break;
	}



	//mPosition.x = METERS_TO_PIXELS((float)position.x);
	//mPosition.y = METERS_TO_PIXELS((float)position.y);
	return true;
}

bool Player::Update()
{
	int speed = 3;
	state = PlayerState::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		state = PlayerState::WDOWN;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
		state = PlayerState::WUP;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		state = PlayerState::WLEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		state = PlayerState::WRIGHT;
	}

	//also move collider

	//position.x = PIXEL_TO_METERS((int)mPosition.x);
	//position.y = PIXEL_TO_METERS((int)mPosition.y);
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
	iPoint Displacement = { 8, 21};
	AnimationState();
	app->render->DrawTexture(texture, position.x - Displacement.x, position.y - Displacement.y, &currentAnim->GetCurrentFrame());

	//draw map that must be drawed after the player
	app->map->DrawAfterPlayer();
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
			npcRange = true;
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
			cheastRange = true;
			break;
		case Collider::Type::CHANGESCENE:
			//FUNCTION TO CHANGE SCENE LOL
			app->sceneManager->changeMap = true;
			if (app->sceneManager->currentScene != 0) {
				break;
			}
			switch (c2->scene)
			{
			case 0:
				break;
			case 1:
				app->sceneManager->resistance_base = true;
				break;
			case 2:
				app->sceneManager->store = true;
				break;
			case 3:
				app->sceneManager->tabern = true;
				break;
			default:
				break;
			}
			break;
		case Collider::Type::ITEM:
			app->moduleCollisions->collision_solver(c1->listener, c2->rect);
		default:
			break;
		}
	}

}

void Player::EndCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::PLAYER)
	{
		switch (c2->type)
		{
		case Collider::Type::NONE:
			break;
		case Collider::Type::WALL:
			break;
		case Collider::Type::ENEMY:
			break;
		case Collider::Type::NPC:
			break;
		case Collider::Type::NPCINTERACTION:
			npcRange = false;
			break;
		case Collider::Type::CAMLIMIT:
			break;
		case Collider::Type::CHEAST:
			break;
		case Collider::Type::CHEASTINTERACTION:
			cheastRange = false;
			break;
		case Collider::Type::CHANGESCENE:
			break;
		case Collider::Type::ITEM:
			break;
		default:
			break;
		}
	}

}

void Player::AnimationState()
{
	switch (state)
	{
	case PlayerState::IDLE:
		currentAnim = &idleAnim;
		break;
	case PlayerState::WUP:
		currentAnim = &walkUpAnim;
		break;
	case PlayerState::WLEFT:
		currentAnim = &walkLeftAnim;
		break;
	case PlayerState::WRIGHT:
		currentAnim = &walkRightAnim;
		break;
	case PlayerState::WDOWN:
		currentAnim = &walkDownAnim;
		break;
	case PlayerState::DYING:
		currentAnim = &dieAnim;
		break;
	case PlayerState::UNKNOWN:
		break;
	default:
		break;
	}
	currentAnim->Update();
}
