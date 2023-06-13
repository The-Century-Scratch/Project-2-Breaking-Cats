#include "ModuleCollisions.h"

#include "App.h"
#include "Debug.h"

#include "Log.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"
#include "SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions(bool startEnabled) : Module(startEnabled)
{
	name.Create("moduleCollisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::WALL][Collider::Type::NPC] = true;
	matrix[Collider::Type::WALL][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::WALL][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::WALL][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::WALL][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::WALL][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::WALL][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::WALL][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::WALL][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::NPC] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::NPCINTERACTION] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CAMLIMIT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHEAST] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHEASTINTERACTION] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHANGESCENE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::MOVABLEOBJECT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::TRIGGERABLEOBJECT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::COLLECTIBLEOBJECT] = true;


	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::NPC] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::NPC][Collider::Type::WALL] = true;
	matrix[Collider::Type::NPC][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NPC][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NPC][Collider::Type::NPC] = false;
	matrix[Collider::Type::NPC][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::NPC][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::NPC][Collider::Type::CHEAST] = true;
	matrix[Collider::Type::NPC][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::NPC][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::NPC][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::NPC][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::NPC][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::NPCINTERACTION][Collider::Type::WALL] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::NPC] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::CAMLIMIT][Collider::Type::WALL] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::NPC] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::CHEAST][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHEAST][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::NPC] = true;
	matrix[Collider::Type::CHEAST][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::NPC] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::CHANGESCENE][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::NPC] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::WALL] = true;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::NPC] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::MOVABLEOBJECT][Collider::Type::COLLECTIBLEOBJECT] = false;

	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::WALL] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::NPC] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::TRIGGERABLEOBJECT][Collider::Type::COLLECTIBLEOBJECT] = false;


	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::WALL] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::NPC] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::CHANGESCENE] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::MOVABLEOBJECT] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::TRIGGERABLEOBJECT] = false;
	matrix[Collider::Type::COLLECTIBLEOBJECT][Collider::Type::COLLECTIBLEOBJECT] = false;

						  
	/*debug = true;*/
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->Intersects(c2->rect))
			{
				c1->onCollision.Add(c2);
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);				
			}
			ListItem<Collider*>* colliderItem = c1->onCollision.start;
			while (colliderItem != NULL)
			{
				if (!c1->Intersects(c2->rect))
				{
					if (matrix[c1->type][c2->type] && c1->listener)
						c1->listener->EndCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->listener)
						c2->listener->EndCollision(c2, c1);
				}
				colliderItem = colliderItem->next;
			}
		}
	}

	return true;
}

bool ModuleCollisions::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		debug = !debug;
	}


	return true;
}

bool ModuleCollisions::PostUpdate()
{
	if(app->debug->drawColliders)
	{
		DebugDraw();
	}

	return true;
}

void ModuleCollisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha, true);			//white
			break;
		case Collider::Type::WALL:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha, true);			//yellow
			break;
		case Collider::Type::PLAYER:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha, true);				//blue
			break;
		case Collider::Type::ENEMY:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha, true);				//red
			break;
		case Collider::Type::NPC:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha, true);				//green
			break;
		case Collider::Type::NPCINTERACTION:
			app->render->DrawRectangle(colliders[i]->rect, 100, 255, 100, alpha * 0.25, true);	//green but lower oppacity
			break;
		case Collider::Type::CAMLIMIT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha * 0.25, true);	//white but lower oppacity
			break;
		case Collider::Type::CHEAST:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 255, alpha, true);			//pink
			break;
		case Collider::Type::CHEASTINTERACTION:
			app->render->DrawRectangle(colliders[i]->rect, 255, 100, 255, alpha * 0.25, true);	//pink but lower oppacity
			break;
		case Collider::Type::CHANGESCENE:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 0, alpha, true);				//black
			break;
		case Collider::Type::MOVABLEOBJECT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha * 0.25, true);		//red but lower oppacity
			break;
		case Collider::Type::TRIGGERABLEOBJECT:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha * 0.25, true);		//blue but lower oppacity
			break;
		case Collider::Type::COLLECTIBLEOBJECT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 140, 0, alpha , true);			//orange
			break;
		default:
			break;
		}

	}
	
}

bool ModuleCollisions::isWalkable(iPoint pos)
{
	for (auto& i : colliders)
	{
		//i->rect.x
		if (i != nullptr && pos.x == i->rect.x && pos.y == i->rect.y)
			return false;
	}

	return true;
}

iPoint ModuleCollisions::CornerPosition()
{
	iPoint pos = { 2133234,463634 };

	for (auto& i : colliders)
	{
		if (i != nullptr)
		{
			
			if (i->rect.x < pos.x || i->rect.y < pos.y)
			{
				pos.x = i->rect.x;
				pos.y = i->rect.y;

			}
		}
		

	}
	return pos;


}

// Called before quitting
bool ModuleCollisions::CleanUp(bool mapColliderOnly)
{
	LOG("Freeing all colliders");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (mapColliderOnly)
		{
			if (colliders[i] != nullptr)
			{
				if (colliders[i]->mapCollider)
				{
					delete colliders[i];
					colliders[i] = nullptr;
				}
			}
		}
		else
		{
			if (colliders[i] != nullptr)
			{
				delete colliders[i];
				colliders[i] = nullptr;
			}
		}
	}

	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Entity* listener, bool mapCollider, int newscene)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{

		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			LOG("Collider number %d created", i);
			ret->scene = newscene;
			if (mapCollider)
			{
				ret->mapCollider = true;
			}

			break;
		}
		else if (i == MAX_COLLIDERS - 1)
		{
			LOG("Max Colliders, coultn't be possible to create more *INCREASE MAX_COLLIDERS VALUE TO AVOID THIS ERROR*");
		}

	}

	return ret;
}


void ModuleCollisions::collision_solver(Entity* element, SDL_Rect element_to_check)
{

	// Calculate the minimum and maximum x and y values for the two rectangles
	int element_left = element->position.x;
	int element_right = element->position.x + element->w;
	int element_top = element->position.y;
	int element_bottom = element->position.y + element->h;

	int check_left = element_to_check.x;
	int check_right = element_to_check.x + element_to_check.w;
	int check_top = element_to_check.y;
	int check_bottom = element_to_check.y + element_to_check.h;

	int left_distance = check_right - element_left;
	int right_distance = check_left - element_right;
	int top_distance = check_bottom - element_top;
	int bottom_distance = check_top - element_bottom;

	// Find the smallest absolute distance and move the element in that direction
	int min_distance = std::min(std::abs(left_distance), std::min(std::abs(right_distance),
		std::min(std::abs(top_distance), std::abs(bottom_distance))));
	if (min_distance == std::abs(left_distance)) {
		element->position.x += left_distance;
	}
	else if (min_distance == std::abs(right_distance)) {
		element->position.x += right_distance;
	}
	else if (min_distance == std::abs(top_distance)) {
		element->position.y += top_distance;
	}
	else {
		element->position.y += bottom_distance;
	}
}

void ModuleCollisions::camera_follow(Collider* camera, SDL_Rect player)
{
	if (player.x < camera->rect.x) {
		// El jugador se ha movido a la izquierda de la cámara
		camera->UpdatePos(-1, 0);
	}
	else if (player.x + player.w > camera->rect.x + camera->rect.w) {
		// El jugador se ha movido a la derecha de la cámara
		camera->UpdatePos(1, 0);
	}
	if (player.y < camera->rect.y) {
		// El jugador se ha movido arriba de la cámara
		camera->UpdatePos(0, -1);
	}
	else if (player.y + player.h > camera->rect.y + camera->rect.h) {
		// El jugador se ha movido abajo de la cámara
		camera->UpdatePos(0, 1);
	}
}
