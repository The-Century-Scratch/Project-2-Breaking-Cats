#include "Scene_Title.h"

#include "Log.h"
#include "Render.h"

bool Scene_Title::isReady()
{
	return true;
}

void Scene_Title::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const &windowFactory, std::string const fileToLoad)
{
	auto sceneHash = info.find("Title");
	if (sceneHash == info.end())
	{
		LOG("Title scene not found in XML.");
		return;
	}

	auto scene = sceneHash->second;

	for (auto const& window : scene.children("window"))
	{
		if (auto result = windowFactory.CreateWindow(window.attribute("name").as_string());
			result != nullptr)
		{
			windows.push_back(std::move(result));
		}
	}

	backgroundTexture = app->tex->Load("Assets/Textures/TitleScreen2.png");
}

void Scene_Title::Start()
{
}

void Scene_Title::Draw()
{
	app->render->DrawTexture(DrawParameters(backgroundTexture, { 0,0 }));
	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Title::Update()
{
	for (auto const& elem : windows)
	{
		if (auto result = elem->Update();
			result != 0)
			return result;
	}
	return 0;
}

int Scene_Title::CheckNextScene(int ret)
{
	/*using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}*/

	return ret;
}
