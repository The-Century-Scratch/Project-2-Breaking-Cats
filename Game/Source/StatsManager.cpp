#include "Catska.h"
#include "Gats.h"

#include "StatsManager.h"

StatsManager::StatsManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("StatsManager");
}

StatsManager::~StatsManager(){}

bool StatsManager::Start()
{
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());
	for (pugi::xml_node nodeUnit = config.child("gats");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("gats"))
	{

		std::unique_ptr<Unit> gats;
		gats = std::make_unique<Gats>();
		gats.get()->velocity = nodeUnit.attribute("velocity").as_int();
		gats.get()->parameters = nodeUnit;
		gats.get()->playerId = nodeUnit.attribute("id").as_int();
		gats.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(gats));
	}
	for (pugi::xml_node nodeUnit = config.child("catska");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("catska"))
	{

		std::unique_ptr<Unit> catska;
		catska = std::make_unique<Catska>();
		catska.get()->velocity = nodeUnit.attribute("velocity").as_int();
		catska.get()->parameters = nodeUnit;
		catska.get()->playerId = nodeUnit.attribute("id").as_int();
		catska.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(catska));
	}
	return true;
}

bool StatsManager::Update(float dt)
{
 	for (auto& i : units)
	{
		if (i.get()->GetPlayerId() == 1)
		{
			gats_n = "GATS";
			gats_hp = i.get()->GetHealthPoints();
			gats_dmg = i.get()->GetDamage();
		}
		else if (i.get()->GetPlayerId() == 2)
		{
			catska_n = "CATSKA";
			catska_hp = i.get()->GetHealthPoints();
			catska_dmg = i.get()->GetDamage();
		}
	}
	return true;
}

bool StatsManager::PostUpdate()
{
	return true;
}

bool StatsManager::CleanUp()
{
	return true;
}