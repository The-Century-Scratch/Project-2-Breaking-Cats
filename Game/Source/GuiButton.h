#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiElement.h"
#include "GuiPanelSegmented.h"
#include "TextureManager.h"

#include <memory>
#include <string>

enum class ButtonState
{
	DISABLED,
	NORMAL,
	PRESSED,
	FOCUSED,
	SELECTED
};

class GuiButton : public GuiElement
{
public:
	GuiButton() = default;
	~GuiButton() override;


	explicit GuiButton(
		uPoint pos,
		uPoint size,
		uint separation,
		std::string const &str,
		std::function<int()> const& funcPtr,
		std::vector<SDL_Rect> const& buttonStates,
		std::string const& path
	);

	int Update() override;
	bool Draw() const override;

	void MouseEnterHandler() override;
	void MouseLeaveHandler() override;

	void DebugDraw() const;

private:
    std::string text = "";
	iPoint type = {0,0};
	int font = 0;

	std::map<int, GuiPanelSegmented> panels;

	iPoint offset;

	ButtonState currentState = ButtonState::DISABLED;
};

#endif __GUIBUTTON_H__