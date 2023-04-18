#include "GuiImage.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "TextManager.h"

#include "Log.h"

#include <map>

GuiImage::~GuiImage()
{
	for (auto const &[index, panel] : panels)
	{
		panel.Unload();
	}
}

GuiImage::GuiImage(uPoint pos, uPoint size, uint separation, std::string const &str, std::function<int()> const& funcPtr, std::vector<SDL_Rect> const &buttonStates, std::string const& path) :
	text(str)/*,
	currentState(ButtonState::NORMAL)*/
{
	Initialize(funcPtr, pos, size);

	int textureID = app->tex->Load(path);

	for (int i = 0; auto const& elem : buttonStates)
	{
		panels.try_emplace(i, elem, separation, textureID, iPoint(3, 3));
		i++;
	}

	if (StrEquals(path, "Assets/UI/ButtonState9.png"))
	{
		type = { 7 , 15 };
	}
}

int GuiImage::Update()
{
	return 0;
}

bool GuiImage::Draw() const
{
	/*auto centerPoint = iPoint(GetPosition().x, GetPosition().y);

	if (auto result = panels.find(static_cast<int>(currentState));
		result != panels.end())
	{
		result->second.Draw(centerPoint, iPoint(GetSize().x, GetSize().y));
	}

	centerPoint += iPoint((GetSize().x/2) - type.x, (GetSize().y/2) - type.y);

	TextParameters params(0, DrawParameters(0, centerPoint));
	params.align = AlignTo::ALIGN_CENTER;

	app->fonts->DrawText(
		text,
		params
	);*/

	return true;
}

void GuiImage::DebugDraw() const
{
	SDL_Rect debugRect(GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);

	app->render->DrawShape(debugRect, false, SDL_Color(255, 0, 0, 255));
}
