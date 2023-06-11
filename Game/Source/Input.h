#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "SDL/include/SDL.h"



#define CONTROLLERUP (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->l_y < -0.5)
#define CONTROLLERDOWN (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->l_y > 0.5)
#define CONTROLLERLEFT (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->l_x < -0.5)
#define CONTROLLERRIGHT (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->l_x > 0.5)

#define CONTROLLERUPONCE (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
#define CONTROLLERDOWNONCE (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
#define CONTROLLERLEFTONCE (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN)
#define CONTROLLERRIGHTONCE (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN)

#define CONTROLLERA (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
#define CONTROLLERB (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
#define CONTROLLERX (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
#define CONTROLLERY (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)

#define CONTROLLERSTART (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_BUTTONS 15
//#define LAST_KEYS_PRESSED_BUFFER 50

struct _SDL_GameController;
struct _SDL_Haptic;

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class GamePad
{
public:
	GamePad();
	virtual ~GamePad();

	inline KeyState GetButton(int id) const
	{
		if (this != nullptr) return buttons[id];
	}

	// Activates SDL device funcionallity when a gamepad has been connected
	void HandleDeviceConnection(int index);

	// Deactivates SDL device funcionallity when a gamepad has been disconnected
	void HandleDeviceRemoval(int index);

	// Called at PreUpdate
	// Iterates through all active gamepad and update all input data
	void UpdateGamepadInput();

	bool ShakeController(int id, int duration, float strength = 0.5f);
	const char* GetControllerName(int id) const;

public:
	//Input data
	bool start, back, guide;
	bool x, y, a, b, l1, r1, l3, r3;
	bool up, down, left, right;
	float l2, r2;
	float l_x, l_y, r_x, r_y, l_dz, r_dz;

	KeyState* buttons;
	//SDL_GameControllerButton* btns;
	SDL_GameControllerButton btns[MAX_BUTTONS];


	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller;
	_SDL_Haptic* haptic;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;
};

class Input : public Module
{

public:

	Input(bool startEnabled);

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	int GetMouseX() const
	{
		return mouseX;
	}

	int GetMouseY() const
	{
		return mouseY;
	}

	GamePad* pad;

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};

#endif // __INPUT_H__