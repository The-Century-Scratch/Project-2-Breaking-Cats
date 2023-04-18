#include "Event_Dialogue.h"

#include "Log.h"

#include <format>
#include <string>

int Event_Dialogue::ReturnType()
{
	return 0;
}

EventProperties::LootProperty Event_Dialogue::getLootProperties() const
{
	EventProperties::LootProperty data; //TODO
	return data;
}

EventProperties::DialogueProperty Event_Dialogue::getDialogueProperties() const
{
	return dialogue;
}

void Event_Dialogue::parseXMLProperties(pugi::xml_node const& node)
{
	for (auto const& child : node.children())
	{
		auto attributeName = child.attribute("name").as_string();

		if (StrEquals("isLocked", attributeName))
		{
			isLocked = child.attribute("value").as_bool();
		}
		else if (StrEquals("Scripts", attributeName))
		{
			dialogue.ReadProperty(child);
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
		else
		{
			LOG("Dialogue property %s not implemented.", attributeName);
		}




	}
}

void Event_Dialogue::Create(pugi::xml_node const& node)
{
	Transform::Initialize(node);
	Sprite::Initialize(node);
	Event_Base::Initialize(node);
}