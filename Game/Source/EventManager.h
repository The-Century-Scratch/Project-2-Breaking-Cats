#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Event_Base.h"

#include <memory>
#include <vector>
#include <string>

struct TileInfo;

class EventManager
{
public:

	EventManager();

	// Destructor
	~EventManager();

	void Initialize();

	// ------ Event
	// --- Constructors
	bool CreateEvent(pugi::xml_node const &node = pugi::xml_node());

	int GetEventLayerSize() const;

	iPoint GetEventVector();

	bool isEvent(iPoint pos) const;

	//EventType getEventType()

	// Returns Gid, position, keepDrawing ? true : false;
	std::tuple<int, iPoint, bool> GetDrawEventInfo(int index = 0);
private:
	std::vector<std::unique_ptr<Event_Base>> events;
	std::vector<std::unique_ptr<Event_Base>> enemies;


	std::vector<std::unique_ptr<Event_Base>>::const_iterator drawIterator;
	std::vector<std::unique_ptr<Event_Base>>::const_iterator enemyReturnIterator;
};


#endif // __ENTITYMANAGER_H__
