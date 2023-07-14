#include "Colliders.h"

Collider::Collider(SDL_Rect rectangle, Type type, Entity* listener) : rect(rectangle), type(type), listener(listener)
{

}

void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Collider::UpdatePos(int x, int y) {
	rect.x += x;
	rect.y += y;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	// returns true if there has been an intersection
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

void Collider::ParseMapProperties(pugi::xml_node const& node)
{
	using CT = Collider::Type;
	pugi::xml_node propertiesNode = node.child("properties");

	if(type != NULL)
	switch (type)
	{
	case CT::CHANGESCENE:

		for (auto const& child : propertiesNode.children("property"))
		{
			auto attributeName = child.attribute("name").as_string();
			if (SDL_strcmp("DestinationMap", attributeName) == 0)
			{
				properties.destinationMap = static_cast<MapType>(child.attribute("value").as_int());
			}
			else if (SDL_strcmp("Destination X", attributeName) == 0)
			{
				properties.destination.x = child.attribute("value").as_int();
			}
			else if (SDL_strcmp("Destination Y", attributeName) == 0)
			{
				properties.destination.y = child.attribute("value").as_int();
			}
		}
		break;
	default:
		break;
	}
}
