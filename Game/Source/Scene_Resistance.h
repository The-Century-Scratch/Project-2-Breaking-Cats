#ifndef __SCENE_RESISTANCE_H__
#define __SCENE_RESISTANCE_H__

#include "Scene_Base.h"
#include "Player.h"
#include "Map.h"
#include "TextureManager.h"

class Scene_Resistance : public Scene_Base
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
    int CheckNextScene(int ret) override;
    void DrawPause() override;
    int Test();

private:
    std::string currentMap = "";

    Map map;
    Player player;
    int pauseMenu;
};


#endif __SCENE_RESISTANCE_H__