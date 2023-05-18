#ifndef __FONT_H__
#define __FONT_H__

#include "Defs.h"

#include "SDL/include/SDL.h"
#include"App.h"
class Textures;

class Font
{
public:

	Font(App* App, const char* rtpFontFile);

	~Font();

	SDL_Texture* GetTextureAtlas();
	SDL_Rect GetCharRec(int value);

	inline int GetBaseSize() { return baseSize; }

	bool UnLoad(Textures* tex);

private:

	bool fontLoaded;

	SDL_Texture* texture;
	int baseSize;
	int charsCount;
	SDL_Rect charsRecs[128];
};

#endif //__FONT_H__