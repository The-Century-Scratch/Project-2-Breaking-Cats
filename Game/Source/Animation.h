#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "App.h"

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 120

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
		loopCount = 0;
	}
	
	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void SetCurrentFrame(int frame) {
		currentFrame = frame;
	}

	void AnimateCat32x32x8(int y)
	{
		PushBack({ 0, y * 32, 32, 32 });
		PushBack({ 32 * 1, y * 32, 32, 32 });
		PushBack({ 32 * 2, y * 32, 32, 32 });
		PushBack({ 32 * 3, y * 32, 32, 32 });
		PushBack({ 32 * 4, y * 32, 32, 32 });
		PushBack({ 32 * 5, y * 32, 32, 32 });
		PushBack({ 32 * 6, y * 32, 32, 32 });
		PushBack({ 32 * 7, y * 32, 32, 32 });
	}

	void AnimateCat32x32x4(int y)
	{
		PushBack({ 0, y * 32, 32, 32 });
		PushBack({ 32 * 1, y * 32, 32, 32 });
		PushBack({ 32 * 2, y * 32, 32, 32 });
		PushBack({ 32 * 3, y * 32, 32, 32 });
	}
};

#endif