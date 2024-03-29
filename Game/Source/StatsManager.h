#pragma once

#include "EASTL/list.h"

#include "Module.h"
#include "Unit.h"
#include <chrono>
#include <memory>
#include <vector>

using namespace std::chrono;

class Player;

class StatsManager : public Module
{
public:

	StatsManager(bool startEnabled);
	virtual ~StatsManager();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

public:
	std::vector<std::unique_ptr<Unit>> units;
	List<Player*> playerList;
	
	SString gats_n;
	int gats_hp;
	int gats_dmg;

	SString catska_n;
	int catska_hp;
	int catska_dmg;

private:
	
};