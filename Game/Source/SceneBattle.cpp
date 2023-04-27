#include "App.h"
#include "Render.h"
//#include"ParticlesManager.h"
#include "Textures.h"
//#include"AssetsManager.h"
#include "SceneBattle.h"
#include "SceneGameplay.h"
#include "Map.h"
//#include "BattleMenu.h"
#include "Player.h"
//#include "Enemy.h"
//#include "Golem.h"
//#include "Skull.h"
//#include "Bat.h"
#include "Audio.h"
//#include "Inventory.h"
#include <time.h>

SceneBattle::SceneBattle(eastl::list<Player*> list, Enemy* enemy, SceneGameplay* s, Inventory* inventory) : playerList(list), scene(s)
{
	//battleMenu = new BattleMenu(this, inventory);
	//map = new Map();
	//
	//if (enemy != nullptr)
	//{
	//	enemy->SetCurrentState(EnemyState::NORMAL);
	//	enemy->SetPos(iPoint(650, 140));
	//	enemyList.push_back(enemy);
	//}
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{
	//particles = new ParticlesManager();
	//if (scene->isDungeon == true)
	//{
	//	map->Load("battle_map2.tmx", app->tex);
	//}
	//else
	//{
	//	map->Load("battle_map.tmx", app->tex);
	//}

	//font = new Font(app, "Font/font3.xml", app->tex);

	////backgroundTexture = app->tex->Load("Assets/Textures/Scenes/battle_bg.png");

	//// Start music
	//app->audio->PlayMusic("Audio/Music/fight_theme_2.ogg", true);
	//
	//eastl::list<Player*>::iterator it = playerList.begin();
	//for (int i = 0; it != playerList.end(); ++it, ++i)
	//{
	//	(*it)->stance = PlayerStance::BATTLE;
	//	(*it)->SetIdleBattle();
	//	(*it)->bounds.x = 400;
	//	(*it)->bounds.y = 150 + (i * 50);
	//	(*it)->battlePos = iPoint((*it)->bounds.x, (*it)->bounds.y);
	//	(*it)->generatorList(particles, 1);
	//}

	//pugi::xml_document animations;
	//pugi::xml_node anims;
	//int size = app->assetsManager->MakeLoad("Xml/animations.xml");
	//pugi::xml_parse_result result = animations.load_buffer(app->assetsManager->GetLastBuffer(), size);
	//app->assetsManager->DeleteBuffer();
	////pugi::xml_parse_result result = animations.load_file("animations.xml");

	//if (result == NULL)
	//	LOG("Could not load xml file: %s. pugi error: %s", "animations.xml", result.description());
	//else
	//	anims = animations.child("animations");

	//Enemy* enemy = nullptr;
	//particles->CreateGenerator({ 0,0 }, ParticleType::FIRE);
	//particles->CreateGenerator({ 0,0 }, ParticleType::MAGIC);
	//particles->CreateGenerator({ 0,0 }, ParticleType::GRAVITY);
	//for (int i = 0; i < 2; ++i)
	//{
	//	int num = (rand() % 3) + 1;
	//	switch(num)
	//	{
	//	case 1:
	//		enemy = new Golem(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	case 2:
	//		enemy = new Skull(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	case 3:
	//		enemy = new Bat(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	}
	//	//(rand()%2)
	//	enemy->generatorList(particles, 1);

	//	if (enemy != nullptr) enemyList.push_back(enemy);
	//}
	//
	//battleMenu->Load(font);
	//
	//app->render->camera.x = 0;
	//app->render->camera.y = 0;

	return true;
}

bool SceneBattle::Update(float dt)
{
	bool ret = true;
	//particles->PreUpdate();
	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->generatorList(particles, (rand() % 3));
	//	(*item)->Update(dt);
	//}

	//eastl::list<Enemy*>::iterator it = enemyList.begin();
	//for (; it != enemyList.end(); ++it)
	//{
	//	(*it)->generatorList(particles, (rand() % 3));
	//	(*it)->Update(dt);
	//}
	//particles->Update(dt);
	//if (battleMenu->Update(dt) == false) ret = false;

	return ret;
}

void SceneBattle::Draw(bool colliders)
{
	////app->render->DrawTexture(backgroundTexture,0,0);
	//map->Draw(colliders,nullptr, nullptr);

	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->Draw(colliders);
	//}
	//
	//eastl::list<Enemy*>::iterator it = enemyList.begin();
	//for (; it != enemyList.end(); ++it)
	//{
	//	(*it)->Draw(colliders);
	//}
	//particles->PostUpdate();
	//battleMenu->Draw(font, colliders);
}

bool SceneBattle::UnLoad()
{
	//font->UnLoad(app->tex);
	//RELEASE(font);

	//battleMenu->UnLoad();
	//RELEASE(battleMenu);

	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->stance = PlayerStance::ROAMING;
	//	(*item)->SetIdleRoaming();
	//}

	//playerList.clear();
	//particles->CleanUp();
	//RELEASE(particles)
	//enemyList.clear();
	//
	//map->CleanUp();
	//RELEASE(map);

	return true;
}