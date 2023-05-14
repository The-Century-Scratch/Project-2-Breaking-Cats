#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

Audio::Audio(bool startEnabled) : Module(startEnabled)
{
	music = NULL;
	name.Create("audio");
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		state = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		state = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		state = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if (!state)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	UnLoadFxs();
	Mix_AllocateChannels(0);
	//channels.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if (!state)
		return false;

	if (music != NULL)
	{
		if (fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fadeTime > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!state)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!state)
		return false;

	if (id > 0 && id <= fx.size())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

bool Audio::UnLoadFx(int index)
{
	if (index > 0 && index <= fx.size())
	{
		Mix_FreeChunk(fx.at(index - 1));
		eastl::erase(fx, fx.at(index - 1));
	}

	return true;
}

void Audio::SetDistanceFx(int channel, int angle, uint distance, uint maxDistance)
{
	distance = distance * 255 / maxDistance;
	if (distance > 255) distance = 255;
	Mix_SetPosition(channel, angle, distance);
}

void Audio::Reset()
{
	Mix_FreeMusic(music);
	fx.clear();
}

void Audio::SetMusicVolume(int index)
{
	musicVolume = index;
	Mix_VolumeMusic(musicVolume);
}

void Audio::SetFxVolume(int index)
{
	fxVolume = index;
}

int Audio::GetMusicVolume()
{
	return musicVolume;
}

int Audio::GetFxVolume()
{
	return fxVolume;
}

void Audio::UnLoadFxs()
{
	eastl::deque<Mix_Chunk*>::iterator item;
	eastl::deque<Mix_Chunk*>::iterator itEnd = fx.end();
	for (item = fx.begin(); item != itEnd; ++item)
		Mix_FreeChunk(*item);

	fx.clear();
}

int Audio::SetChannel()
{
	int ret = -1;
	++numChannels;
	if (numChannels < maxChannels)
	{
		return numChannels;
	}
	else
	{
		maxChannels += 10;
		Mix_AllocateChannels(maxChannels);
		return numChannels;
	}

	return ret;
}