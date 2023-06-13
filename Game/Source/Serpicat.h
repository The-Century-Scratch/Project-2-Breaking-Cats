#ifndef __SERPICAT_H__
#define __SERPICAT_H__

#include "Sprite.h"
#include "Transform.h"
#include "Point.h"
#include "Unit.h"



class Serpicat : public Unit
{
public:
	Serpicat();
	~Serpicat();

	void DebugDraw() const override;

	PlayerAction HandleInput() const;
	void StartAction(PlayerAction playerAction) override;

private:
};

#endif //__SERPICAT_H__
