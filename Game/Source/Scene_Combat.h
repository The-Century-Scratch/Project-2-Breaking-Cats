#ifndef __SCENE_COMBAT_H__
#define __SCENE_COMBAT_H__

#include "Scene_Base.h"
#include "Player.h"
#include "Unit.h"
#include "Straw.h"
#include "Gats.h"
#include "Catska.h"
#include "LongRange.h"
#include "Map.h"

enum class UnitType
{
    GUARDIAN,
    STRAW,
    GATS,
    CATSKA,
    LONGRANGE,
    NONE
};

class Scene_Combat : public Scene_Base
{
public:
    bool isReady() override;
    void Load(
        std::string const& path,
        LookUpXMLNodeFromString const& info,
        Window_Factory const& windowFactory,
        std::string const fileToLoad = "Base"
    ) override;
    void Start() override;
    void Draw() override;
    int Update() override;
    int CheckNextScene() override;

    void CreateUnit();

    void LoadEnemies(const std::string& directory, const std::string& level);

private:
    std::string currentMap = "";

    Map map;
    Player player;


    std::vector<std::unique_ptr<Unit>> units;

    int turn = 0;
    int numberFinished = 0;
};


#endif __SCENE_COMBAT_H__
