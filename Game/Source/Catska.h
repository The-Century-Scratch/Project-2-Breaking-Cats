#ifndef __CATSKA_H__
#define __CATSKA_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Catska : public Unit
{
public:
	Catska();
	~Catska();
	void Create(iPoint pos) override;

	void Draw() const override;
	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

	bool GetIsAlly() override;

	void DealDamage(int amount);

private:
};

#endif //__PLAYER_H__
