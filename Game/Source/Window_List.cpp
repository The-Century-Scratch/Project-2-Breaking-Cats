#include "Window_List.h"

#include "Log.h"

Window_List::Window_List(pugi::xml_node const& node) : Window_Base(node)
{
	AddFunctionToMap("NewGame", std::bind_front(&Window_List::NewGame, this));
	AddFunctionToMap("ContinueGame", std::bind_front(&Window_List::ContinueGame, this));
	AddFunctionToMap("ExitGame", std::bind_front(&Window_List::ExitGame, this));
	AddFunctionToMap("CombatGame", std::bind_front(&Window_List::CombatGame, this));
	AddFunctionToMap("VerticalGame", std::bind_front(&Window_List::VerticalGame, this));
	CreateButtons(node);
}

int Window_List::NewGame()
{
	LOG("NewGame function called");
	return 1;
}

int Window_List::ContinueGame()
{
	LOG("ContinueGame function called");
	return 2;
}

int Window_List::ExitGame()
{
	LOG("ExitGame function called");
	return 3;
}

int Window_List::CombatGame()
{
	LOG("CombatGame function called");
	return 4;
}

int Window_List::VerticalGame()
{
	LOG("Vertical Game function called");
	return 5;
}
