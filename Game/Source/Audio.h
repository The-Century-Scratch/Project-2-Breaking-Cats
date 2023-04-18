#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include <memory>

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define DEFAULT_VOLUME_MUSIC 100
#define LOWERED_VOLUME_MUSIC 30

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Set music volume to any value
	bool SetVolumeMusic(int newVolume = DEFAULT_VOLUME_MUSIC);

	// Pause/resume music
	bool PauseMusic(bool pause = true);

private:

	_Mix_Music* music = nullptr;
	std::list<std::unique_ptr<Mix_Chunk>> fx;
};

#endif // __AUDIO_H__