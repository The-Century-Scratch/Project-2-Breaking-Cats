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
#include "Inventory.h"
#include "ParticleSystemManager.h"

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
	portal1          = { 0, TILE_H * 2, TILE_W, TILE_H };
	portal2          = { TILE_W, TILE_H * 2, TILE_W, TILE_H };

	focusedAnim.PushBack({ 0, TILE_H, TILE_W, TILE_H });
	focusedAnim.PushBack({ TILE_W, TILE_H, TILE_W, TILE_H });
	focusedAnim.loop = true;
	focusedAnim.speed = 0.05f;


	gridPos = { 16,16 }; // TODO: que lo loadee de la propia escena/ del tmx
	showArea = false;

	lastPortal = { 0,0 };
	firstPortal = { 0,0 };

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

	if (!lastPortal.IsZero())
		app->render->DrawTexture(gridTex,lastPortal.x, lastPortal.y, &portal1);
	if (!firstPortal.IsZero())
		app->render->DrawTexture(gridTex, firstPortal.x, firstPortal.y, &portal2);
}

bool GridSystem::CleanUp()
{
	if (firstPortalPS != nullptr)
	{
		firstPortalPS->TurnOff();
		firstPortalPS = nullptr;
	}
	if (lastPortalPS != nullptr)
	{
		lastPortalPS->TurnOff();
		lastPortalPS = nullptr;
	}
	app->tex->Unload(gridTex);
	return true;
}

bool GridSystem::isWalkable(iPoint pos)
{
	if (pos.x < TILE_W || pos.y < TILE_H) return false;

	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (x >= MAX_TILES_X || y >= MAX_TILES_Y) return false;

	if(grid[x][y].walkability == TileWalkability::WALKABLE) return true;

	return false;
}

bool GridSystem::isUnit(iPoint pos)
{
	if (pos.x < TILE_W || pos.y < TILE_H) return false;

	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (x >= MAX_TILES_X || y >= MAX_TILES_Y) return false;

	if (grid[x][y].walkability == TileWalkability::UNIT) return true;

	return false;
}

bool GridSystem::isPortal(iPoint pos)
{
	if (pos.x < TILE_W || pos.y < TILE_H) return false;

	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (x >= MAX_TILES_X || y >= MAX_TILES_Y) return false;

	if (pos == lastPortal || pos == firstPortal) return true;

	return false;
}

bool GridSystem::isPortalsActivated()
{
	if (!lastPortal.IsZero() && !firstPortal.IsZero())
		return true;

	return false;
}

void GridSystem::move(iPoint origin, iPoint destination)
{
	for (auto& u : unitsData)
	{
		if (u->unitPos == origin) 
			u->unitPos = destination;
	}

	int x = (origin.x - gridPos.x) / TILE_W;
	int y = (origin.y - gridPos.y) / TILE_H;

	grid[x][y].walkability = TileWalkability::WALKABLE;

	x = (destination.x - gridPos.x) / TILE_W;
	y = (destination.y - gridPos.y) / TILE_H;

	grid[x][y].walkability = TileWalkability::UNIT;
}

void GridSystem::removeUnit(iPoint UnitPos)
{
	int x = (UnitPos.x - gridPos.x) / TILE_W;
	int y = (UnitPos.y - gridPos.y) / TILE_H;

	grid[x][y].walkability = TileWalkability::WALKABLE;
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
	iPoint aoePos = { 0,0 };
	for (size_t x = 0; x < MAX_TILES_X; x++)
	{
		for (size_t y = 0; y < MAX_TILES_Y; y++)
		{
			if (grid[x][y].state == TileState::AREA_EFFECT && !grid[x][y].isFocused)
			{
				grid[x][y].state = TileState::UNSELECTED;
			}

			if ((grid[x][y].state == TileState::CLICKABLE || grid[x][y].state == TileState::AREA_EFFECT) && showArea)
			{
				if (IsMouseInside(grid[x][y].bounds)/* && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT*/)
				{
					aoePos = { (int)x + gridPos.x,(int)y + gridPos.y };
				}  
			}
			else
			{
				grid[x][y].state = TileState::UNSELECTED;
			}

			if (IsMouseInside(grid[x][y].bounds))
			{
				grid[x][y].isFocused = true;
				focusPos = grid[x][y].bounds;
			}
			else
				grid[x][y].isFocused = false;
		}
	}

	if (!aoePos.IsZero())
	{
		showEffectArea(grid[aoePos.x - gridPos.x][aoePos.y - gridPos.y].bounds);
	}
}

TileWalkability GridSystem::LoadWalkabilityfromCollisions(iPoint pos)
{
	TileWalkability tempWalkability = TileWalkability::UNWALKABLE;

	if (!app->moduleCollisions->isWalkable(pos)) tempWalkability = TileWalkability::OBSTACLE;
	else tempWalkability = TileWalkability::WALKABLE;
	for (auto& u : unitsData)
	{
		if (u.get()->unitPos == pos)
			tempWalkability = TileWalkability::UNIT;
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

bool GridSystem::AreaIsClicked()
{
	int x = (focusPos.x - gridPos.x) / TILE_W;
	int y = (focusPos.y - gridPos.y) / TILE_H;

	if (IsMouseInside(focusPos) &&
		app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN &&
		grid[x][y].state == TileState::AREA_EFFECT)
		return true;

	return false;
}

eastl::vector<iPoint> GridSystem::getHitsPosition()
{
	eastl::vector<iPoint> Hits;

	// Acciones que necesitan pasar el objetivo
	if (currentAction.action == Unit::PlayerAction::Action::PREPARE_DASH || 
		currentAction.action == Unit::PlayerAction::Action::ATTACK_LONG_RANGE || 
		currentAction.action == Unit::PlayerAction::Action::GRENADE ||
		currentAction.action == Unit::PlayerAction::Action::SILLYMAGIC)
	{
		iPoint dashDestination = { focusPos.x, focusPos.y };
		Hits.push_back(eastl::move(dashDestination));
	}

	// Acciones que solo necesitan 1 casilla
	if (currentAction.action == Unit::PlayerAction::Action::TELEPORT ||
		currentAction.action == Unit::PlayerAction::Action::PORTAL)
	{
		iPoint dashDestination = { focusPos.x, focusPos.y };
		Hits.push_back(eastl::move(dashDestination));
		return Hits;
	}

	for (size_t x = 0; x < MAX_TILES_X; x++)
	{
		for (size_t y = 0; y < MAX_TILES_Y; y++)
		{
			if (grid[x][y].state == TileState::AREA_EFFECT && grid[x][y].walkability == TileWalkability::UNIT)
			{
				iPoint tempPos = { grid[x][y].bounds.x, grid[x][y].bounds.y };
				Hits.push_back(eastl::move(tempPos));
			}
		}
	}

	return Hits;
}

iPoint GridSystem::getFocusPosition()
{
	return iPoint(focusPos.x, focusPos.y);
}

void GridSystem::PlacePortal(iPoint pos)
{
	if (firstPortalPS != nullptr)
	{
		firstPortalPS->TurnOff();
		firstPortalPS = nullptr;
	}
	if (lastPortalPS != nullptr)
	{
		lastPortalPS->TurnOff();
		lastPortalPS = nullptr;
	}

	firstPortal = lastPortal;
	lastPortal = pos;

	if (!lastPortal.IsZero())
	{
		lastPortalPS = app->particleManager->CreateParticleSystem(lastPortal, Blueprint::PORTAL1, { lastPortal.x + TILE_W, lastPortal.y + TILE_H });
	}

	if (!firstPortal.IsZero())
	{
		firstPortalPS = app->particleManager->CreateParticleSystem(firstPortal, Blueprint::PORTAL2, { firstPortal.x + TILE_W, firstPortal.y + TILE_H });
	}
}

iPoint GridSystem::getOtherPortal(iPoint pos)
{
	if (pos == lastPortal)
		return firstPortal;
	if (pos == firstPortal)
		return lastPortal;

	return iPoint(0, 0);
}

void GridSystem::showActionArea()
{
	using UA = Unit::PlayerAction::Action;

	switch (currentAction.action)
	{
	case UA::ATTACK:
		showAttack(currentAction.destinationTile);
		break;
	case UA::ATTACK_LONG_RANGE:
		showAttackRange(currentAction.destinationTile);
		break;
	case UA::PREPARE_DASH:
		showDash(currentAction.destinationTile);
		break;
	case UA::ATTACK_AND_HEAL_WITH_KILL:
		showAttack(currentAction.destinationTile);
		break;
	case UA::GRENADE:
		showGrenade(currentAction.destinationTile);
		break;
	case UA::TELEPORT:
		showTeleport(currentAction.destinationTile);
		break;
	case UA::PORTAL:
		showPortal(currentAction.destinationTile);
		break;
	case UA::SILLYMAGIC:
		showSillyMagic(currentAction.destinationTile);
		break;
	default:
		break;
	}
}

void GridSystem::showEffectArea(SDL_Rect r)
{
	using UA = Unit::PlayerAction::Action;

	iPoint pos = { r.x,r.y };

	switch (currentAction.action)
	{
	case UA::ATTACK:
		showAttackAOE(pos);
		break;
	case UA::ATTACK_LONG_RANGE:
		showAttackRangeAOE(pos);
		break;
	case UA::PREPARE_DASH:
		showDashAOE(pos);
		break;
	case UA::ATTACK_AND_HEAL_WITH_KILL:
		showAttackAOE(pos);
		break;
	case UA::GRENADE:
		showGrenadeAOE(pos);
		break;
	case UA::TELEPORT:
		showTeleportAOE(pos);
		break;
	case UA::PORTAL:
		showPortalAOE(pos);
		break;
	case UA::SILLYMAGIC:
		showSillyMagicAOE(pos);
		break;
	default:
		break;
	}
}


void GridSystem::showAttack(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (x - 1 >= 0)
	grid[x - 1][y].state = TileState::CLICKABLE;
	if (x + 1 < MAX_TILES_X)
	grid[x + 1][y].state = TileState::CLICKABLE;
	if (y - 1 >= 0)
	grid[x][y - 1].state = TileState::CLICKABLE;
	if (y + 1 < MAX_TILES_Y)
	grid[x][y + 1].state = TileState::CLICKABLE;
}

void GridSystem::showAttackRange(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	for (int i = 0; i < MAX_TILES_X; i++)
	{
		if (grid[i][y].walkability == TileWalkability::OBSTACLE || i == x) continue;
		grid[i][y].state = TileState::CLICKABLE;
	}

	for (int j = 0; j < MAX_TILES_Y; j++)
	{
		if (grid[x][j].walkability == TileWalkability::OBSTACLE || j == y) continue;
		grid[x][j].state = TileState::CLICKABLE;
	}
}

void GridSystem::showDash(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	for (int i = x - 3; i <= x + 3 && i < 9; i++)
	{
		if (grid[i][y].walkability != TileWalkability::WALKABLE || i == x || i < 0) continue;

		grid[i][y].state = TileState::CLICKABLE;
	}

	for (int j =  y - 3; j <= y + 3 && j < 9; j++)
	{
		if (grid[x][j].walkability != TileWalkability::WALKABLE || j == y || j < 0) continue;

		grid[x][j].state = TileState::CLICKABLE;
	}
}

void GridSystem::showGrenade(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	iPoint unitPos = { grid[x][y].bounds.x , grid[x][y].bounds.y };

	for (size_t i = 0; i < MAX_TILES_X; i++)
	{
		for (size_t j = 0; j < MAX_TILES_Y; j++)
		{
			if (grid[i][j].walkability == TileWalkability::OBSTACLE) continue;
			iPoint tilePos = { grid[i][j].bounds.x , grid[i][j].bounds.y };
			if (unitPos.DistanceTo(tilePos) < 5 * TILE_W && unitPos.DistanceTo(tilePos) > 3 * TILE_W)
			{
				grid[i][j].state = TileState::CLICKABLE;
			}
		}
	}
}

void GridSystem::showTeleport(iPoint pos)
{
	for (size_t i = 0; i < MAX_TILES_X; i++)
	{
		for (size_t j = 0; j < MAX_TILES_Y; j++)
		{
			if (grid[i][j].walkability == TileWalkability::WALKABLE)
				grid[i][j].state = TileState::CLICKABLE;
		}
	}
}

void GridSystem::showPortal(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	iPoint unitPos = { grid[x][y].bounds.x , grid[x][y].bounds.y };

	for (size_t i = 0; i < MAX_TILES_X; i++)
	{
		for (size_t j = 0; j < MAX_TILES_Y; j++)
		{
			iPoint tilePos = { grid[i][j].bounds.x , grid[i][j].bounds.y };
			if (grid[i][j].walkability != TileWalkability::WALKABLE || tilePos == lastPortal) continue;
			if (unitPos.DistanceTo(tilePos) < 5 * TILE_W)
			{
				grid[i][j].state = TileState::CLICKABLE;
			}
		}
	}
}

void GridSystem::showSillyMagic(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	iPoint unitPos = { grid[x][y].bounds.x , grid[x][y].bounds.y };

	for (size_t i = 0; i < MAX_TILES_X; i++)
	{
		for (size_t j = 0; j < MAX_TILES_Y; j++)
		{
			iPoint tilePos = { grid[i][j].bounds.x , grid[i][j].bounds.y };
			if (grid[i][j].walkability == TileWalkability::OBSTACLE) continue;
			if (unitPos.DistanceTo(tilePos) < 3 * TILE_W)
			{
				grid[i][j].state = TileState::CLICKABLE;
			}
		}
	}
}

void GridSystem::showAttackAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;
	
	if (pos.x > currentAction.destinationTile.x || pos.x < currentAction.destinationTile.x)
	{
		if (grid[x][y - 1].walkability != TileWalkability::OBSTACLE && y - 1 >= 0)
			grid[x][y - 1].state = TileState::AREA_EFFECT;

		if (grid[x][y + 1].walkability != TileWalkability::OBSTACLE && y + 1 < MAX_TILES_Y)
			grid[x][y + 1].state = TileState::AREA_EFFECT;

		if (grid[x][y].walkability != TileWalkability::OBSTACLE)
			grid[x][y].state = TileState::AREA_EFFECT;
	}
	else if (pos.y > currentAction.destinationTile.y || pos.y < currentAction.destinationTile.y)
	{
		if (grid[x - 1][y].walkability != TileWalkability::OBSTACLE && x - 1 >= 0)
			grid[x - 1][y].state = TileState::AREA_EFFECT;

		if (grid[x + 1][y].walkability != TileWalkability::OBSTACLE && x + 1 < MAX_TILES_X)
			grid[x + 1][y].state = TileState::AREA_EFFECT;

		if (grid[x][y].walkability != TileWalkability::OBSTACLE)
			grid[x][y].state = TileState::AREA_EFFECT;
	}
}

void GridSystem::showAttackRangeAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	if (pos.x > currentAction.destinationTile.x)
	{
		for (int i = x; (grid[i][y].walkability == TileWalkability::WALKABLE || grid[i][y].walkability == TileWalkability::UNIT) && grid[i][y].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9; i++)
		{
			grid[i][y].state = TileState::AREA_EFFECT;
			if (grid[i][y].walkability == TileWalkability::UNIT && !app->inventory->GetBulletPenetration()) break;
		}
	}
	else if (pos.x < currentAction.destinationTile.x)
	{
		for (int i = x; (grid[i][y].walkability == TileWalkability::WALKABLE || grid[i][y].walkability == TileWalkability::UNIT) && grid[i][y].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9; i--)
		{
			grid[i][y].state = TileState::AREA_EFFECT;
			if (grid[i][y].walkability == TileWalkability::UNIT && !app->inventory->GetBulletPenetration()) break;
		}
	}
	else if (pos.y > currentAction.destinationTile.y)
	{
		for (int i = y; (grid[x][i].walkability == TileWalkability::WALKABLE || grid[x][i].walkability == TileWalkability::UNIT) && grid[x][i].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9; i++)
		{
			grid[x][i].state = TileState::AREA_EFFECT;
			if (grid[x][i].walkability == TileWalkability::UNIT && !app->inventory->GetBulletPenetration()) break;
		}
	}
	else if (pos.y < currentAction.destinationTile.y)
	{
		for (int i = y; (grid[x][i].walkability == TileWalkability::WALKABLE || grid[x][i].walkability == TileWalkability::UNIT) && grid[x][i].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9; i--)
		{
			grid[x][i].state = TileState::AREA_EFFECT;
			if (grid[x][i].walkability == TileWalkability::UNIT && !app->inventory->GetBulletPenetration()) break;
		}
	}
}

void GridSystem::showDashAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	int a = (currentAction.destinationTile.x - gridPos.x) / TILE_W;
	int b = (currentAction.destinationTile.y - gridPos.y) / TILE_H;

	if (pos.x > currentAction.destinationTile.x)
	{
		for (int i = x; (grid[i][y].walkability == TileWalkability::WALKABLE || grid[i][y].walkability == TileWalkability::UNIT) && grid[i][y].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9 && i >= a; i--)
		{
			grid[i][y].state = TileState::AREA_EFFECT;
		}
	}
	else if (pos.x < currentAction.destinationTile.x)
	{
		for (int i = x; (grid[i][y].walkability == TileWalkability::WALKABLE || grid[i][y].walkability == TileWalkability::UNIT) && grid[i][y].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9 && i <= a; i++)
		{
			grid[i][y].state = TileState::AREA_EFFECT;
		}
	}
	else if (pos.y > currentAction.destinationTile.y)
	{
		for (int i = y; (grid[x][i].walkability == TileWalkability::WALKABLE || grid[x][i].walkability == TileWalkability::UNIT) && grid[x][i].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9 && i >= b; i--)
		{
			grid[x][i].state = TileState::AREA_EFFECT;
		}
	}
	else if (pos.y < currentAction.destinationTile.y)
	{
		for (int i = y; (grid[x][i].walkability == TileWalkability::WALKABLE || grid[x][i].walkability == TileWalkability::UNIT) && grid[x][i].walkability != TileWalkability::OBSTACLE && i >= 0 && i < 9 && i <= b; i++)
		{
			grid[x][i].state = TileState::AREA_EFFECT;
		}
	}
}

void GridSystem::showGrenadeAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	grid[x][y].state = TileState::AREA_EFFECT;

	if (grid[x + 1][y].walkability != TileWalkability::OBSTACLE && x + 1 >= 0 && x + 1 < 9)
		grid[x + 1][y].state = TileState::AREA_EFFECT;
	if (grid[x - 1][y].walkability != TileWalkability::OBSTACLE && x - 1 >= 0 && x - 1 < 9)
		grid[x - 1][y].state = TileState::AREA_EFFECT;
	if (grid[x][y + 1].walkability != TileWalkability::OBSTACLE && y + 1 >= 0 && y + 1 < 9)
		grid[x][y + 1].state = TileState::AREA_EFFECT;
	if (grid[x][y - 1].walkability != TileWalkability::OBSTACLE && y - 1 >= 0 && y - 1 < 9)
		grid[x][y - 1].state = TileState::AREA_EFFECT;

}

void GridSystem::showTeleportAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	grid[x][y].state = TileState::AREA_EFFECT;
}

void GridSystem::showPortalAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	grid[x][y].state = TileState::AREA_EFFECT;
}

void GridSystem::showSillyMagicAOE(iPoint pos)
{
	int x = (pos.x - gridPos.x) / TILE_W;
	int y = (pos.y - gridPos.y) / TILE_H;

	grid[x][y].state = TileState::AREA_EFFECT;

	if (isPortal(pos))
	{
		if (grid[x + 1][y].walkability != TileWalkability::OBSTACLE && x + 1 >= 0 && x + 1 < 9)
			grid[x + 1][y].state = TileState::AREA_EFFECT;
		if (grid[x - 1][y].walkability != TileWalkability::OBSTACLE && x - 1 >= 0 && x - 1 < 9)
			grid[x - 1][y].state = TileState::AREA_EFFECT;
		if (grid[x + 1][y + 1].walkability != TileWalkability::OBSTACLE && x + 1 >= 0 && x + 1 < 9)
			grid[x + 1][y + 1].state = TileState::AREA_EFFECT;
		if (grid[x - 1][y + 1].walkability != TileWalkability::OBSTACLE && x - 1 >= 0 && x - 1 < 9)
			grid[x - 1][y + 1].state = TileState::AREA_EFFECT;
		if (grid[x][y + 1].walkability != TileWalkability::OBSTACLE && y + 1 >= 0 && y + 1 < 9)
			grid[x][y + 1].state = TileState::AREA_EFFECT;
		if (grid[x][y - 1].walkability != TileWalkability::OBSTACLE && y - 1 >= 0 && y - 1 < 9)
			grid[x][y - 1].state = TileState::AREA_EFFECT;
		if (grid[x + 1][y - 1].walkability != TileWalkability::OBSTACLE && y + 1 >= 0 && y + 1 < 9)
			grid[x + 1][y - 1].state = TileState::AREA_EFFECT;
		if (grid[x - 1][y - 1].walkability != TileWalkability::OBSTACLE && y - 1 >= 0 && y - 1 < 9)
			grid[x - 1][y - 1].state = TileState::AREA_EFFECT;
	}
}
