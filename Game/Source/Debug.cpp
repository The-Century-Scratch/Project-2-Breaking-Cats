#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Debug.h"
#include "Scene.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Defs.h"
#include "Log.h"
#include "SceneGameplay.h"

#include <string>

Debug::Debug(bool startEnabled) : Module(startEnabled) 
{
	debug = false;
	name.Create("debug");
}

Debug::~Debug() {}

bool Debug::Start()
{
	debug = false;
	desiredFPS = 60;
	debugColor = { 0, 0, 0, 0 };
	//font = new Font(app, "Font/CleanCraters.xml", app->tex);

	return true;
}

bool Debug::Update(float dt)
{
	int scale = app->win->GetScale();

	debugX = (app->render->camera.w / scale) + 10;
	debugY = (app->render->camera.h / scale) + 10;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug)
	{
		godMode = true;
		drawColliders = true;
		drawHugBounds = true;
		drawVariables = true;
	}
	else
	{
		godMode = false;
		drawColliders = false;
		drawHugBounds = false;
		drawVariables = false;
	}

	return true;
}

bool Debug::PostUpdate()
{
	//SDL_Color white = { 0,0,0,0 };
	//app->render->DrawText("pau Mongolooooooooo", 100, 100, 100, 20, white);
	//DrawDebug();

	return true;
}

void Debug::DrawDebug()
{
	int scale = app->win->GetScale();

	debugX = app->render->camera.w / scale * 0.7;
	debugY = app->render->camera.h / scale * 0.1;

	//app->fonts->BlitText(debugX, debugY, 0, "variables (v)");

	////Camera Limits
	//if (camLimits)
	//	app->fonts->BlitText(debugX, debugY + 10, 0, "camera limits (c)  on");
	//else
	//	app->fonts->BlitText(debugX, debugY + 10, 0, "camera limits (c)  off");

	
	SDL_Rect rect = { debugX, debugY, 200, 25 };
	//app->render->DrawRectangle(rect, 255, 0, 0, 255, true, true);
	SDL_Color color = {255,255,255,255};
	//Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint, int maxX
	//app->render->DrawText(font, "pau mongolooooooo", debugX, debugY, 100, 10, color);
	//app->render->DrawText("pau Mongolooooooooo", debugX+2, debugY+2, 100, 20, color);
	

	////Variables
	//if (variables)
	//{	
	//	//Free Camera
	//	if (freeCam)
	//		app->fonts->BlitText(debugX, debugY + 30, 0, "#free cam (f8)  on");
	//	else
	//		app->fonts->BlitText(debugX, debugY + 30, 0, "#free cam (f8)  off");
	//	
	//	//God Mode
	//	if (godMode)
	//		app->fonts->BlitText(debugX, debugY + 40, 0, "#god mode (f10)  on");
	//	else
	//		app->fonts->BlitText(debugX, debugY + 40, 0, "#god mode (f10)  off");

	//	//Player x, y
	//	app->fonts->BlitText(debugX , debugY + 55, 0, "player.x =");
	//	app->fonts->BlitText(debugX + 88, debugY + 55, 0, std::to_string(app->scene->player->position.x).c_str());

	//	app->fonts->BlitText(debugX, debugY + 65, 0, "player.y =");
	//	app->fonts->BlitText(debugX + 88, debugY + 65, 0, std::to_string(app->scene->player->position.y).c_str());

	//	//Camera x, y
	//	app->fonts->BlitText(debugX, debugY + 80, 0, "camera.x =");
	//	app->fonts->BlitText(debugX + 88, debugY + 80, 0, std::to_string(app->render->camera.x).c_str());

	//	app->fonts->BlitText(debugX, debugY + 90, 0, "camera.y =");
	//	app->fonts->BlitText(debugX + 88, debugY + 90, 0, std::to_string(app->render->camera.y).c_str());

	//	//Player alive
	//	if (app->scene->player->alive)
	//		app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = true");
	//	else
	//		app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = false");
	//}

	////Camera limits
	//if (camLimits)
	//{
	//	int scale = app->win->GetScale();

	//	app->scene->rectCamera.x = app->render->camera.w * 0.4;
	//	app->scene->rectCamera.y = app->render->camera.h * 0.4;
	//	app->scene->rectCamera.w = app->render->camera.w * 0.2;
	//	app->scene->rectCamera.h = app->render->camera.h * 0.2;
	//	app->render->DrawRectangle(app->scene->rectCamera, 0, 255, 0, 255, false, false);
	//}

	////Teleport
	//if (teleport)
	//{
	//	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//	{
	//		app->scene->player->position.x = 2120;
	//		app->scene->player->position.y = 385;
	//	}
	//}
}

void Debug::DrawColliders()
{
	//b2World* world = app->physics->GetWorld();

	//// Bonus code: this will iterate all objects in the world and draw the circles
	//// You need to provide your own macro to translate meters to pixels
	//for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	//{
	//	for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
	//	{
	//		switch (f->GetType())
	//		{
	//			// Draw circles ------------------------------------------------
	//		case b2Shape::e_circle:
	//		{
	//			b2CircleShape* shape = (b2CircleShape*)f->GetShape();
	//			uint width, height;
	//			app->win->GetWindowSize(width, height);
	//			b2Vec2 pos = f->GetBody()->GetPosition();
	//			app->render->DrawCircle(METERS_TO_PIXELS(pos.x) * app->win->GetScale(),
	//				METERS_TO_PIXELS(pos.y) * app->win->GetScale(),
	//				METERS_TO_PIXELS(shape->m_radius) * app->win->GetScale(), 255, 255, 255);
	//		}
	//		break;

	//		// Draw polygons ------------------------------------------------
	//		case b2Shape::e_polygon:
	//		{
	//			b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
	//			int32 count = polygonShape->GetVertexCount();
	//			b2Vec2 prev, v;

	//			for (int32 i = 0; i < count; ++i)
	//			{
	//				v = b->GetWorldPoint(polygonShape->GetVertex(i));
	//				if (i > 0)
	//					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 255, 100);

	//				prev = v;
	//			}

	//			v = b->GetWorldPoint(polygonShape->GetVertex(0));
	//			app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
	//		}
	//		break;

	//		// Draw chains contour -------------------------------------------
	//		case b2Shape::e_chain:
	//		{
	//			b2ChainShape* shape = (b2ChainShape*)f->GetShape();
	//			b2Vec2 prev, v;

	//			for (int32 i = 0; i < shape->m_count; ++i)
	//			{
	//				v = b->GetWorldPoint(shape->m_vertices[i]);
	//				if (i > 0)
	//					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
	//				prev = v;
	//			}

	//			v = b->GetWorldPoint(shape->m_vertices[0]);
	//			app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
	//		}
	//		break;

	//		// Draw a single segment(edge) ----------------------------------
	//		case b2Shape::e_edge:
	//		{
	//			b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
	//			b2Vec2 v1, v2;

	//			v1 = b->GetWorldPoint(shape->m_vertex0);
	//			v1 = b->GetWorldPoint(shape->m_vertex1);
	//			app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
	//		}
	//		break;
	//		}

	//		// TODO 1: If mouse button 1 is pressed ...
	//		// app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
	//		// test if the current body contains mouse position
	//	}
	//}
}