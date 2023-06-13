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

	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

	void DealDamage(int amount);

	void SpecificAnimationState();

private:
	Animation attackLeftAnim;
	Animation attackRightAnim;
};

#endif //__PLAYER_H__
