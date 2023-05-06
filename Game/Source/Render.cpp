#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render(bool startEnabled) : Module(startEnabled)
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, bool ignoreScale, int forceScale) const
{
	bool ret = true;
	uint scale = app->win->GetScale();
	if (ignoreScale)
	{
		scale = forceScale;
	}

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int posx, int posy, int w, int h, SDL_Color color) {

	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { posx, posy, w, h };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return true;
}

bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint, int maxX) const
{
	bool ret = true;

	bool noMoreJumps = false;

	int length = strlen(text);
	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	eastl::string tmp;
	int tmpLen = 0;
	int firstTmpLetterPosition = 0;
	for (int i = 0; i < length + 1; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };

		if (text[i] != ' ' && text[i] != '\0')
		{
			tmp.push_back(text[i]);
			++tmpLen;

			if (tmpLen == 1 && maxX > 0) firstTmpLetterPosition = posX; /*+ recGlyph.w;*/

			if (text[i + 1] == ' ')
			{
				tmp.push_back(text[i + 1]);
				++tmpLen;
			}

			if (maxX > 0)
			{
				if ((tmpLen * (recDest.w + spacing)) + firstTmpLetterPosition >= maxX && noMoreJumps == false)
				{
					noMoreJumps = true;
					y += size + 5;
					posX = x;
				}
			}
		}
		else // Print the text
		{
			for (int j = 0; j < tmp.length(); ++j)
			{
				recGlyph = font->GetCharRec(tmp[j]);
				recDest.x = posX;
				recDest.w = scale * recGlyph.w;
				SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);
				posX += ((float)recGlyph.w * scale + spacing);
			}
			firstTmpLetterPosition = posX;
			tmp.clear();
			tmpLen = 0;
			noMoreJumps = false;
		}
	}

	return ret;
}

bool Render::DrawText(Font* font, const char* text, SDL_Rect bounds, int size, int spacing, SDL_Color tint, int maxX) const
{
	bool ret = true;

	bool noMoreJumps = false;

	int length = strlen(text);
	int posX = bounds.x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);
	SDL_SetTextureAlphaMod(font->GetTextureAtlas(), tint.a);
	//SDL_SetRenderDrawColor(renderer, tint.r, tint.g, tint.b, tint.a);

	eastl::string tmp;
	int tmpLen = 0;
	int firstTmpLetterPosition = 0;
	for (int i = 0; i < length + 1; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, bounds.y, (scale * recGlyph.w), size };

		if (text[i] != ' ' && text[i] != '\0')
		{
			tmp.push_back(text[i]);
			++tmpLen;

			if (tmpLen == 1 && maxX > 0) firstTmpLetterPosition = posX; /*+ recGlyph.w;*/

			if (text[i + 1] == ' ')
			{
				tmp.push_back(text[i + 1]);
				++tmpLen;
			}

			if (maxX > 0)
			{
				if ((tmpLen * (recDest.w + spacing)) + firstTmpLetterPosition >= maxX && noMoreJumps == false)
				{
					noMoreJumps = true;
					bounds.y += size + 5;
					posX = bounds.x;
					//bounds.h += size + 5;
				}
			}
		}
		else // Print the text
		{
			for (int j = 0; j < tmp.length(); ++j)
			{
				recGlyph = font->GetCharRec(tmp[j]);
				recDest.x = posX;
				recDest.w = scale * recGlyph.w;
				SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);
				posX += ((float)recGlyph.w * scale + spacing);
			}
			firstTmpLetterPosition = posX;
			tmp.clear();
			tmpLen = 0;
			noMoreJumps = false;
		}
	}

	return ret;
}

bool Render::DrawCenterText(Font* font, const char* text, SDL_Rect bounds, int size, int spacing, SDL_Color tint, int maxDistance) const
{
	bool ret = true;

	int length = strlen(text);
	int posX = bounds.x;
	int posY = bounds.y;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);
	int textW = 0;
	for (int i = 0; i < length; ++i)
	{
		textW += ((float)font->GetCharRec(text[i]).w * scale + spacing);
	}

	if (bounds.w != 0)
	{
		int a = (bounds.w - (textW - spacing)) / 2;
		posX = bounds.x + a;
	}
	if (bounds.h != 0)
	{
		int b = (bounds.h - size) / 2;
		posY = bounds.y + b;
	}

	if (size == 64)
		posY += 7;
	else if (size == 36)
		posY += 4;

	int a = (bounds.w - (textW - spacing)) / 2;
	for (int i = 0; i < length; ++i)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, posY, (scale * recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		if (maxDistance > -1)
		{
			if (recDest.x + recDest.w >= maxDistance)
			{
				posY += recDest.h;
				posX = bounds.x + a;
			}
		}

		posX += ((float)recGlyph.w * scale + spacing);

	}

	return ret;
}


// L03: DONE 6: Implement a method to load the state
// for now load camera's x and y
bool Render::LoadState(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Render::SaveState(pugi::xml_node& data)
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

bool Render::DrawParticleAlpha(SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha, float scale, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint windowScale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * windowScale;
	rect.y = (int)(camera.y * speed) + y * windowScale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= windowScale;
	rect.h *= windowScale;

	rect.w *= scale;
	rect.h *= scale;

	rect.x -= rect.w * 0.5f;
	rect.y -= rect.h * 0.5f;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;

	}

	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, alpha);

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}