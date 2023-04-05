#ifndef __EVENT_SPAWNENEMY_H__
#define __EVENT_SPAWNENEMY_H__

#include "Event_Base.h"
#include "Sprite.h"
#include "Transform.h"

#include <memory>

class Event_SpawnEnemy : public Event_Base, public Sprite, public Transform
{
public:
	// Inherited via Scene_Base
	void parseXMLProperties(pugi::xml_node const& node) override;
	// No longer inherited

	void Create(pugi::xml_node const& node) override;
	void Test();
	int ReturnType() override;

private:
	std::vector<std::unique_ptr<EventProperties::LootProperty>> loot;
	std::vector<std::unique_ptr<EventProperties::GlobalSwitchProperty>> globalSwitch;

	bool isLocked = false;
	bool isOpen = false;
};

#endif __EVENT_SPAWNENEMY_H__