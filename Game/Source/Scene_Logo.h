#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

#include "Scene_Base.h"
#include "TextureManager.h"
#include "Log.h"

class Scene_Logo : public Scene_Base
{
public:
    bool isReady() override;
    void Load(
        std::string const& path,
        LookUpXMLNodeFromString const& info,
        Window_Factory const& windowFactory,
        std::string const fileToLoad = ""
    ) override;
    void Start() override;
    void Draw() override;
    int Update() override;
    int CheckNextScene(int ret) override;
    void DrawPause() override;

private:
    int logo;
    int timer;
};

#endif __SCENE_LOGO_H__