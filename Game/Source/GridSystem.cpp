#include "GridSystem.h"
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
#include "SceneBattle.h"

GridSystem::GridSystem()
{
}

GridSystem::~GridSystem() {}

bool GridSystem::Start()
{
	return true;
}

bool GridSystem::Update()
{
	return true;
}

void GridSystem::Draw()
{
}

bool GridSystem::CleanUp()
{
	return true;
}
