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
	int gats_hp = 0;
	int gats_dmg = 0;

	SString catska_n;
	int catska_hp = 0;
	int catska_dmg = 0;

private:
	
};