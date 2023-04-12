#include "EventManager.h"
#include "App.h"

#include "Event_Chest.h"
#include "Event_Teleport.h"
#include "Event_SpawnEnemy.h"

#include "Sprite.h"

#include "Log.h"

EventManager::EventManager() = default;

// Destructor
EventManager::~EventManager() = default;

void EventManager::Initialize()
{
	if (!events.empty())
		drawIterator = events.begin();
}

std::tuple<iPoint, int> EventManager::GetEnemyInfo()
{
	//if (events.size())
	//{
	//	events[0]->name;
	//	events.begin()->get();
	//	std::string hello = "hello";
	//
	//
	//
	//	
	//
	//	//iPoint pos = dynamic_cast<Transform*>(drawIterator->get())->GetPosition();
	//	//LOG("this is the position %i, %i", pos.x, pos.y);
	//
	//
	//	enemyReturnIterator = events.begin();
	//
	//	const char* hi = hello.c_str();  // learn what this function does
	//
	//	int i = 0;
	//
	//	//while (enemyReturnIterator->get() != nullptr)
	//	//{
	//	//	//if (enemyReturnIterator->get()->name.compare("10 Gold"))
	//	//	//if (enemyReturnIterator->get()->common.isActivee)
	//	//	{
	//	//		//LOG("I give up");
	//	//	}
	//	//	//if (enemyReturnIterator->get()->name == "10 Gold")
	//	//	//if (StrEquals(enemyReturnIterator->get()->name, "10 Gold"))
	//	//	{
	//	//		
	//	//		//drawIterator = events.begin();
	//	//		//
	//	//		//drawIterator = events.begin();
//	//	//		hello = enemyReturnIterator->get()->name;
//	//	//		hi = hello.c_str();
//	//	//		LOG("%s, %i", hi, i);
	//	//		
	//	//		//return std::make_tuple(gid, pos, false);
	//	//	}
	//	//
	//	////	hello = enemyReturnIterator->get()->name;
	//	////	hi = hello.c_str();
	//	//	//LOG("%s, %i", hi, i);
	//	//	++i;
	//	//
	//	//	++enemyReturnIterator;
	//	//} 
	//
	//
	//	//	events[0]->name;
//	//events.begin()->get();
//	//std::string hello = "hello";
//	//
//	//enemyReturnIterator = events.begin();
//	//
//	//const char* hi = hello.c_str();  // learn what this function does
//	//
//	//int i = 0;
//	//
//	//while (enemyReturnIterator->get() != nullptr)
//	//{
//	//
//	//	if (enemyReturnIterator->get()->name == "Unlock Chest")
//	//	{
//	//		//drawIterator = events.begin();
//	//		//
//	//		//drawIterator = events.begin();
//	//
//	//		//return std::make_tuple(gid, pos, false);
//	//	}
//	//
//	//	hello = enemyReturnIterator->get()->name;
//	//	hi = hello.c_str();
//	//	LOG("%s, %i", hi, i);
//	//	++i;
//	//
//	//	++enemyReturnIterator;
//	//}
	//
	//	
	//	//drawIterator = events.begin();
	//	//hello = drawIterator->get()->name;
	//	//hi = hello.c_str();
	//	//LOG("%s", hi);
	//
	//
	//
	//
	//
	//
	//	//iPoint pos = dynamic_cast<Transform*>(drawIterator->get())->GetPosition();
	//	//LOG("this is the position %i, %i", pos.x, pos.y);
	//
	//	//return events[0]->name;
	//}


	if (enemies.size())
	{
		auto sth = enemies.size();

		iPoint returnPos = dynamic_cast<Transform*>(enemies[enemies.size() - 1].get())->GetPosition();
		int returnType = enemies[enemies.size() - 1].get()->ReturnType();

		//enemies[enemies.size() - 1].get()

		LOG("it is %i", enemies[enemies.size() - 1].get()->ReturnType());

		enemies.pop_back();
		

		return std::make_tuple(returnPos, returnType);
		//enemyReturnIterator->get()->

		LOG(" %i", sth);
	}
	iPoint pos = { -1,-1 };

	return std::make_tuple(pos, 0);
	
}

bool EventManager::CreateEvent(pugi::xml_node const& node)
{
	for (auto const& child : node.children("object"))
	{
		


		std::unique_ptr<Event_Base> event = nullptr;
		std::unique_ptr<Event_Base> enemy = nullptr;

		if (StrEquals("Event Chest", child.attribute("type").as_string()))
		{
			event = std::make_unique<Event_Chest>();
		}
		else if (StrEquals("Event Teleport", child.attribute("type").as_string()))
		{
			event = std::make_unique<Event_Teleport>();
		}
		else if (StrEquals("Event SpawnEnemy", child.attribute("type").as_string()))
		{
			enemy = std::make_unique<Event_SpawnEnemy>();  // It coould be something like an ordered map used to store them in order depending on their speed, or soomething like that
			//enemy.get()->
			LOG("it is indeed detecting it, now the only problem is spawning it");
			enemy->Create(child);
			enemies.push_back(std::move(enemy));
		}

		if (!event)
		{
			LOG("Event \"%s\" not yet implemented.", child.attribute("type").as_string());
			continue;
		}

		event->Create(child);
		events.push_back(std::move(event));
	}
	return true;
}

int EventManager::GetEventLayerSize() const
{
	return events.size();
}
 
std::tuple<int, iPoint, bool> EventManager::GetDrawEventInfo([[maybe_unused]] int index)
{
	if (events.empty() || drawIterator == events.end())
		return std::make_tuple(0, iPoint(0, 0), false);
	
	auto sprite = dynamic_cast<Sprite*>(drawIterator->get());
	
	if (!sprite)
	{
		++drawIterator;
		
		if (drawIterator == events.end())
		{
			drawIterator = events.begin();
			return std::make_tuple(0, iPoint(0, 0), false);
		}
		return std::make_tuple(0, iPoint(0, 0), true);
	}

	auto gid = sprite->GetGid();

	auto pos = dynamic_cast<Transform*>(drawIterator->get())->GetPosition();

	do {
		++drawIterator;
		if (drawIterator == events.end())
		{
			drawIterator = events.begin();
			return std::make_tuple(gid, pos, false);
		}
	} while (!drawIterator->get()->IsEventActive());

	return std::make_tuple(gid, pos, true);
	
}


