#include "Event_SpawnEnemy.h"

#include "Log.h"

#include <format>
#include <string>

void Event_SpawnEnemy::Test()
{

}

int Event_SpawnEnemy::ReturnType()
{
	return static_cast<int>(typeOfUnit);
	
}

void Event_SpawnEnemy::parseXMLProperties(pugi::xml_node const& node)
{
	for (auto const& child : node.children())
	{
		auto attributeName = child.attribute("name").as_string();

		if (StrEquals("isLocked", attributeName))
		{
			isLocked = child.attribute("value").as_bool();
		}
		else if (StrEquals("isOpen", attributeName))
		{
			isOpen = child.attribute("value").as_bool();
		}
		else if (StrEquals("Base", attributeName))
		{
			common.ReadProperty(child);
		}
		// Loot1, Loot2, Loot3... Way to add multiple loots to a single chest.
		else if (StrEquals(std::format("{}{}", "Loot", std::to_string(loot.size() + 1)), attributeName))
		{
			loot.emplace_back(std::make_unique<EventProperties::LootProperty>());
			loot.back()->ReadProperty(child);
		}
		// GlobalSwitch1, GlobalSwitch2... Way to add multiple global switches to a single chest.
		else if (StrEquals(std::format("{}{}", "GlobalSwitch", std::to_string(globalSwitch.size() + 1)), attributeName))
		{
			globalSwitch.emplace_back(std::make_unique<EventProperties::GlobalSwitchProperty>());
			globalSwitch.back()->ReadProperty(child);
		}
		else if (StrEquals("Type", attributeName))
		{
			
			static_cast<UnitType>(child.attribute("value").as_int());
			int test = child.attribute("value").as_int();
			LOG("this is the value that the enemy recieves from tiled: %i", static_cast<UnitType>(child.attribute("value").as_int()) );
			if (UnitType::STRAW == static_cast<UnitType>(child.attribute("value").as_int()))
			{
				LOG("dafkjl\nfsdakflma\n\n\n\n\nfadsjlfiaiujin\n\n\n");
			}
			typeOfUnit = static_cast<UnitType>(child.attribute("value").as_int());

			//isOpen = child.attribute("value").as_bool();
		}
		else
		{
			LOG("Chest property %s not implemented.", attributeName);
		}
	}
}

void Event_SpawnEnemy::Create(pugi::xml_node const& node)
{
	Transform::Initialize(node);
	Sprite::Initialize(node);
	Event_Base::Initialize(node);
}