#include "GuiButton.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "TextManager.h"

#include "Log.h"

#include <map>

GuiButton::~GuiButton()
{
	for (auto const &[index, panel] : panels)
	{
		panel.Unload();
	}
}

GuiButton::GuiButton(uPoint pos, uPoint size, uint separation, std::string const &str, std::function<int()> const& funcPtr, std::vector<SDL_Rect> const &buttonStates, std::string const& path) :
	text(str),
	currentState(ButtonState::NORMAL)
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

int GuiButton::Update()
{
	using enum ButtonState;
	if (currentState == DISABLED)
		return 0;

	if (IsMouseHovering())
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
		{
			currentState = PRESSED;
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
		{
			return ExecuteFunction();
			currentState = NORMAL;
		}
		else
		{
			currentState = FOCUSED;
		}
	}
	else
	{
		currentState = NORMAL;
	}
	return 0;
}

bool GuiButton::Draw() const
{
	auto centerPoint = iPoint(GetPosition().x, GetPosition().y);

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
	);

	return true;
}

void GuiButton::MouseEnterHandler()
{
	if (currentState != ButtonState::DISABLED)
		currentState = ButtonState::FOCUSED;
}

void GuiButton::MouseLeaveHandler()
{
	if (currentState != ButtonState::DISABLED)
		currentState = ButtonState::NORMAL;
}

void GuiButton::DebugDraw() const
{
	SDL_Rect debugRect(GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);

	app->render->DrawShape(debugRect, false, SDL_Color(255, 0, 0, 255));
}
