#include "ModuleCollisions.h"

#include "App.h"

#include "Log.h"

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

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::NPC] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::NPCINTERACTION] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CAMLIMIT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHEAST] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHEASTINTERACTION] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHANGESCENE] = true;

	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::NPC] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::NPC][Collider::Type::WALL] = true;
	matrix[Collider::Type::NPC][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NPC][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NPC][Collider::Type::NPC] = false;
	matrix[Collider::Type::NPC][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::NPC][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::NPC][Collider::Type::CHEAST] = true;
	matrix[Collider::Type::NPC][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::NPC][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::NPCINTERACTION][Collider::Type::WALL] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::NPC] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::NPCINTERACTION][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::CAMLIMIT][Collider::Type::WALL] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::NPC] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::CHEAST][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHEAST][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::NPC] = true;
	matrix[Collider::Type::CHEAST][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHEAST][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::NPC] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHEASTINTERACTION][Collider::Type::CHANGESCENE] = false;

	matrix[Collider::Type::CHANGESCENE][Collider::Type::WALL] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::NPC] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::NPCINTERACTION] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHEAST] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHEASTINTERACTION] = false;
	matrix[Collider::Type::CHANGESCENE][Collider::Type::CHANGESCENE] = false;
						  
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
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
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
	if (debug)
		DebugDraw();

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
		default:
			break;
		}

	}
	
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Entity* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
		else if (i == MAX_COLLIDERS - 1)
		{
			LOG("Max Colliders, coultn't be possible to create more *INCREASE MAX_COLLIDERS VALUE TO AVOID THIS ERROR*");
		}
	}

	return ret;
}


void ModuleCollisions::collision_solver(Entity* element, Entity* element_to_check)
{
	if (((element_to_check->position.y + (element_to_check->h / 2)) < (element->position.y + (element->h / 2))) && ((abs((element->position.x) - ((element_to_check->position.x) + ((element_to_check->w) / 2))) < ((element_to_check->w) / 2)) && (abs(((element->position.x) + (element->w)) - ((element_to_check->position.x) + ((element_to_check->w) / 2))) < ((element_to_check->w) / 2)))) {
		// TP element to ground surface
		element->position.y = element_to_check->position.y + element_to_check->h;
	}
	else if (((element_to_check->position.y + (element_to_check->h / 2)) >= (element->position.y + (element->h / 2))) && ((abs((element->position.x) - ((element_to_check->position.x) + ((element_to_check->w) / 2))) < ((element_to_check->w) / 2)) && (abs(((element->position.x) + (element->w)) - ((element_to_check->position.x) + ((element_to_check->w) / 2))) < ((element_to_check->w) / 2)))) {
		// TP element to ground bottom
		element->position.y = element_to_check->position.y - element->h;
	}
	else if (((element_to_check->position.x + (element_to_check->w / 2)) < (element->position.x + (element->w / 2))) && ((abs((element->position.y) - ((element_to_check->position.y) + ((element_to_check->h) / 2))) < ((element_to_check->h) / 2)) && (abs(((element->position.y) + (element->h)) - ((element_to_check->position.y) + ((element_to_check->h) / 2))) < ((element_to_check->h) / 2)))) {
		// TP element to ground left
		element->position.x = element_to_check->position.x + element_to_check->w;
	}
	else if (((element_to_check->position.x + (element_to_check->w / 2)) >= (element->position.x + (element->w / 2))) && ((abs((element->position.y) - ((element_to_check->position.y) + ((element_to_check->h) / 2))) < ((element_to_check->h) / 2)) && (abs(((element->position.y) + (element->h)) - ((element_to_check->position.y) + ((element_to_check->h) / 2))) < ((element_to_check->h) / 2)))) {
		// TP element to ground right
		element->position.x = element_to_check->position.x - element->w;
	}
	else if ((element_to_check->position.y + (element_to_check->h / 2)) < (element->position.y + (element->h / 2))) {
		// TP element to ground surface
		element->position.y = element_to_check->position.y + element_to_check->h;
	}
	else if ((element_to_check->position.y + (element_to_check->h / 2)) >= (element->position.y + (element->h / 2))) {
		// TP element to ground bottom
		element->position.y = element_to_check->position.y - element->h;
	}
}