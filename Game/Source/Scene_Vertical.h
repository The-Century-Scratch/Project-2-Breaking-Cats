#ifndef __SCENE_VERTICAL_H__
#define __SCENE_VERTICAL_H__

#include "Scene_Base.h"
#include "Player.h"
#include "Map.h"

class Scene_Vertical : public Scene_Base
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
    int Test();

private:
    std::string currentMap = "";

    Map map;
    Player player;
};


#endif __SCENE_VERTICAL_H__