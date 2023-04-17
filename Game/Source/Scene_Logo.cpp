#include "Scene_Logo.h"
#include "Event_Base.h"

#include "Log.h"
#include "Render.h"

bool Scene_Logo::isReady()
{
	return true;
}

void Scene_Logo::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const &windowFactory, std::string const fileToLoad)
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
}

void Scene_Logo::Start()
{
}

void Scene_Logo::Draw()
{
	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Logo::Update()
{
	for (auto const& elem : windows)
	{
		if (auto result = elem->Update();
			result != 0)
			return result;
	}
	return 0;
}

int Scene_Logo::CheckNextScene(int ret)
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}
	else if (app->input->GetKey(SDL_SCANCODE_C) == KEY_UP)
	{
		return static_cast<int>(SceneType::COMBAT);
	}

	return ret;
}

void Scene_Logo::DrawPause()
{
}
