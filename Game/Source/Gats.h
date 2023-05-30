#ifndef __GATS_H__
#define __GATS_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Gats : public Unit
{
public:
	Gats();
	~Gats();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const override;
	void StartAction(PlayerAction playerAction) override;

	void Test();

private:
	bool goingToDash = false;
};

#endif //__PLAYER_H__
