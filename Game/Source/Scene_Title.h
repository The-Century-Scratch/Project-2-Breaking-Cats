#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Scene_Base.h"
#include "TextureManager.h"
#include "Log.h"
#include "Audio.h"

class Scene_Title : public Scene_Base
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
    int backgroundTexture;
    int music;
};

#endif __SCENE_TITLE_H__