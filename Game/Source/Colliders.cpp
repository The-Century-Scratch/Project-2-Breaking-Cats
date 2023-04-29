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