#ifndef __LONGRANGE_H__
#define __LONGRANGE_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class LongRange : public Unit
{
public:
	LongRange();
	~LongRange();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

private:
};

#endif //__PLAYER_H__
