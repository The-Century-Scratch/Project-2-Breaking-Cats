#ifndef __STRAW_H__
#define __STRAW_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Straw : public Unit
{
public:
	Straw();
	~Straw();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

private:
};

#endif //__PLAYER_H__
