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
#include "Window.h"

GridSystem::GridSystem()
{
}

GridSystem::~GridSystem() {}

bool GridSystem::Start()
{
	/*pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child("GridSystem");*/

	gridTex = app->tex->Load("Assets/Textures/gridTex.png");

	clickableSection = { 0, 0, TILE_W, TILE_H };
	areaSection      = { TILE_W, 0, TILE_W, TILE_H };

	focusedAnim.PushBack({ 0, TILE_H, TILE_W, TILE_H });
	focusedAnim.PushBack({ TILE_W, TILE_H, TILE_W, TILE_H });
	focusedAnim.loop = true;
	focusedAnim.speed = 0.05f;


	gridPos = { 16,16 }; // TODO: que lo loadee de la propia escena/ del tmx


	for (size_t x = 0; x < MAX_TILES_X; x++)
	{
		for (size_t y = 0; y < MAX_TILES_Y; y++)
		{
			grid[x][y].bounds = { TILE_W * (int) x + gridPos.x, TILE_H * (int) y + gridPos.y, TILE_W, TILE_H };
			grid[x][y].state = TileState::UNSELECTED;
			//TODO: tile walkability loadeado de las collisions
			grid[x][y].walkability = LoadWalkabilityfromCollisions({ grid[x][y].bounds.x , grid[x][y].bounds.y });
		}
	}
	return true;
}

bool GridSystem::Update()
{
	HandleTileState();
	return true;
}

void GridSystem::Draw()
{
	DrawTileState();
}

void GridSystem::DrawTileState()
{
	for (size_t x = 0; x < MAX_TILES_X; x++)
	{
		for (size_t y = 0; y < MAX_TILES_Y; y++)
		{
			switch (grid[x][y].state)
			{
			case TileState::UNSELECTED:
				break;
			case TileState::CLICKABLE:
				app->render->DrawTexture(gridTex, grid[x][y].bounds.x, grid[x][y].bounds.y, &clickableSection);
				break;
			case TileState::AREA_EFFECT:
				app->render->DrawTexture(gridTex, grid[x][y].bounds.x, grid[x][y].bounds.y, &areaSection);
				break;
			default:
				break;
			}
			
			if (grid[x][y].isFocused)
			{
				currentAnim = &focusedAnim;
				app->render->DrawTexture(gridTex, grid[x][y].bounds.x, grid[x][y].bounds.y, &currentAnim->GetCurrentFrame());
				currentAnim->Update();
			}

			
		}
	}

}

bool GridSystem::CleanUp()
{
	app->tex->Unload(gridTex);
	return true;
}

bool GridSystem::isWalkable(iPoint pos)
{
	if (pos.x < TILE_W || pos.y < TILE_H) return false;

	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (x > MAX_TILES_X || y > MAX_TILES_Y) return false;

	if(grid[x][y].walkability == TileWalkability::WALKABLE) return true;

	return false;
}

void GridSystem::move(iPoint origin, iPoint destination)
{
	for (auto& u : unitsData)
	{
		if (u->unitPos == origin) u->unitPos = destination;
	}

	int x = (origin.x - gridPos.x) / TILE_W;
	int y = (origin.y - gridPos.y) / TILE_H;

	grid[x][y].walkability == TileWalkability::WALKABLE;

	x = (destination.x - gridPos.x) / TILE_W;
	y = (destination.y - gridPos.y) / TILE_H;

	grid[x][y].walkability == TileWalkability::UNIT;
}

bool GridSystem::IsMouseInside(SDL_Rect r)
{
	int scale = app->win->GetScale();
	SDL_Rect auxR = { r.x + 1, r.y + 1, r.w - 2, r.h - 2 };

	int x, y;
	app->input->GetMousePosition(x, y);

	return (x /*/ scale*/ >= auxR.x) && (x /*/ scale*/ <= auxR.x + auxR.w) && 
		   (y /*/ scale*/ >= auxR.y) && (y /*/ scale*/ <= auxR.y + auxR.h);
}

void GridSystem::HandleTileState()
{
	for (size_t x = 0; x < MAX_TILES_X; x++)
	{
		for (size_t y = 0; y < MAX_TILES_Y; y++)
		{
			if (IsMouseInside(grid[x][y].bounds) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				grid[x][y].state = TileState::AREA_EFFECT;
				grid[x][y].isFocused = true;
			}
			else if (IsMouseInside(grid[x][y].bounds))
			{
				grid[x][y].state = TileState::UNSELECTED;
				grid[x][y].isFocused = true;
			}
			else
			{
				grid[x][y].state = TileState::UNSELECTED;
				grid[x][y].isFocused = false;
			}
		}
	}
}

TileWalkability GridSystem::LoadWalkabilityfromCollisions(iPoint pos)
{
	TileWalkability tempWalkability = TileWalkability::UNWALKABLE;

	if (!app->moduleCollisions->isWalkable(pos)) tempWalkability = TileWalkability::OBSTACLE;
	else tempWalkability = TileWalkability::WALKABLE;
	for (auto& u : unitsData)
	{
		if (u.get()->unitPos == pos) tempWalkability = TileWalkability::UNIT;
	}
	return tempWalkability;
}

void GridSystem::LoadUnitData(Unit* u)
{
	eastl::unique_ptr<UnitData> tempData;
	tempData = eastl::make_unique<UnitData>();

	tempData.get()->type = u->GetType();
	tempData.get()->unitPos = u->position;

	unitsData.push_back(eastl::move(tempData));
}