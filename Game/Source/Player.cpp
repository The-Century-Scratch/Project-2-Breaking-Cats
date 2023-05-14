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

	// idle player
	//idleanim.PushBack({ 0, 0, 32, 32 });
	//idleanim.loop = false;
	//idleanim.speed = 0.0f;

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
	currentAnim = &idleanim;


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
	case 4:
		position.x = 334;
		position.y = 106;
		break;
	case 5:
		position.x = 334;
		position.y = 106;
		break;
	case 6:
		position.x = 136;
		position.y = 23;
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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
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

	app->render->DrawTexture(texture, position.x, position.y);

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
			if (app->sceneManager->currentScene == 0) {
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
				case 5:
					app->sceneManager->downAfterLabrinth = true;
					break;
				case 7:
					app->sceneManager->topPreLab = true;
				}
			}
			if (app->sceneManager->currentScene == 4)
			{
				switch (c2->scene)
				{
				case 5:
					app->sceneManager->leftAfterLabrinth = true;
					app->sceneManager->puzzle1solved = true;
					break;
				case 6:
					app->sceneManager->village = true;
					break;
				default:
					break;
				}
			}
			if (app->sceneManager->currentScene == 5)
			{
				switch (c2->scene)
				{
				case 0:
					app->sceneManager->nordCity = true;
					app->sceneManager->puzzle2solved = true;
					break;
				case 4:
					app->sceneManager->rightLabrinth = true;
					break;
				default:
					break;
				}
			}
			if (app->sceneManager->currentScene == 7)
			{
				switch (c2->scene)
				{
				case 0:
					app->sceneManager->downCity = true;
					break;
				case 8://TO APPLY, THIS WILL BE FUTURE LAB

					app->sceneManager->downCity = true;//MUST BE CHANGED TO FUTURE LAB MAP

					app->sceneManager->puzzle3solved = true;//if you reach lab, it means you solved the 3rd puzzle, that is the invisible labrinth
					break;
				default:
					break;
				}
			}
			break;
		case Collider::Type::MOVABLEOBJECT:
			break;
		case Collider::Type::TRIGGERABLEOBJECT:
			break;
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
		default:
			break;
		}
	}

}