#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#define MAX_COLLIDERS 100

#include "Module.h"
#include "Colliders.h"
#include "Entity.h"

class ModuleCollisions : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	ModuleCollisions(bool startEnabled);

	// Destructor
	~ModuleCollisions();

	// Called at the beginning of the application loop
	// Removes all colliders pending to delete
	// Checks for new collisions and calls its listeners
	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update();

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Entity* listener = nullptr);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	//solves collisions lol
	void collision_solver(Entity* element, SDL_Rect element_to_check);

	//camera keeps track of player if it tries to leave camera zone
	void camera_follow(Collider* camera, SDL_Rect player);

	// Simple debugging flag to draw all colliders
	bool debug = true;

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];

};





#endif // __MODULE_COLLISIONS_H__