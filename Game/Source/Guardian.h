#ifndef __GUARDIAN_H__
#define __GUARDIAN_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Guardian : public Unit
{
public:
	Guardian();
	~Guardian();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const override;
	void StartAction(PlayerAction playerAction) override;

private:
};

#endif //__PLAYER_H__
