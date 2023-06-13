#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"
#include <chrono>
using namespace std::chrono;

#define DEBUG_BOX 225
class Font;
class Debug : public Module
{
public:

	Debug(bool startEnabled);
	virtual ~Debug();

	bool Start();
	bool Update(float dt);	// Switch debug on/off
	bool PostUpdate();		// Draw if debug true

	void DrawDebug();
	void DrawColliders();

public:
	//flags
	bool debug = 0;
	bool godMode = false;
	bool drawColliders = false;
	bool drawHugBounds = false;
	bool drawVariables = false;

	bool camLimits = false;
	bool freeCam = false;
	bool teleport = false;
	bool moveRight = false;
	bool controlFPS = false;
	bool colourblind = false;

	SDL_Color debugColor;
	int debugX = 0;
	int debugY = 0;

	// FPS
	microseconds timePerCycle;
	microseconds elapsedFrame;
	int desiredFPSmic = 0;
	int desiredFPS = 0;
  
private:
	Font* font;
	
};

#endif // __DEBUG_H__