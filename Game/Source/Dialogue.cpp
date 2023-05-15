#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "DialogueManager.h"
#include "Dialogue.h"
#include "Audio.h"
#include "Window.h"

#include <time.h>

DialogueOption::DialogueOption()
{
	id = 0; nextNodeId = 0; isPressed = false; missionId = -1; menu = -1;
}

DialogueOption::~DialogueOption()
{
}

Dialogue::Dialogue(int dialogueId) : id(dialogueId)
{
	isDialogueActive = false;
	/*talkingFx4 = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/CatTalking.wav");*/
	firstTime = true;

	channel = app->audio->SetChannel();
}

Dialogue::~Dialogue()
{
}

void Dialogue::Draw(int& count, Font* font)
{
	// Render npc text like a writing machine
	++count;
	if (count % 2 == 0 && currentNode->stopDialog == false)
	{
		if (currentNode->letterCount < currentNode->text.size())
		{
			textToPrint += currentNode->text.at(currentNode->letterCount);
			++currentNode->letterCount;
			++audioCount;
			if (audioCount == 3) {
				app->audio->PlayFx(channel, talkingFx4);
				audioCount = 0;
			}
		}
		else
		{
			firstTime = true;
			currentNode->dialogFinished = true;
		}
	}

	//TODO: basicamente no hardcodear todo porq sino el scale ira borracho. los draws de abajo estan tremendamente harcodeados cambiarlos con scales y mierdas
	app->render->DrawText(font, textToPrint.c_str(), 40, app->win->height - (56* app->win->scale), 40, 5, { 255,255,255,255 }, 760);
	//app->render->DrawText(textToPrint.c_str(), 70, 500, 1000,40, { 255,255,255,255 });

	if (currentNode->dialogFinished)
	{
		// Render options for the player
		eastl::list<DialogueOption*>::iterator optionsIterator = currentNode->options.begin();

		SDL_Rect r = (*optionsIterator)->bounds;
		int offsetY = 0;
		if (nodes.size() > 1)
		{
			for (; optionsIterator != (*currentNode).options.end(); ++optionsIterator)
			{
				r.y += offsetY;
				if ((*optionsIterator)->isPressed)
				{
					SDL_Rect tmp = r;
					tmp.x += 10;
					app->render->DrawText(font, (*optionsIterator)->text.c_str(), tmp.x + 40, tmp.y + 350,20, 5, { 255,255,255,255 }, 1210);
					//app->render->DrawText((*optionsIterator)->text.c_str(), tmp.x, tmp.y, 30, 5, { 200,50,50,255 });
					
				}
				else
				app->render->DrawText(font, (*optionsIterator)->text.c_str(), r.x + 40, r.y + 350, 20, 5, { 255,255,255,255 }, 1210);
				//app->render->DrawText((*optionsIterator)->text.c_str(), r.x, r.y, 30, 5, { 20,20,20,255 });
					
				offsetY += 90;
			}
		}
		else
		{
			//if(isPressed)
			//	app->render->DrawText(font, (*optionsIterator)->text.c_str(), (*optionsIterator)->bounds, 30, 5, { 70,70,70,255 }, 1210);
			//else
			//app->render->DrawText(font, (*optionsIterator)->text.c_str(), (*optionsIterator)->bounds, 30, 5, { 20,20,20,255 }, 1210);

			if ((*optionsIterator)->isPressed)
			{
				SDL_Rect tmp = (*optionsIterator)->bounds;
				tmp.x += 10;
				app->render->DrawText(font, (*optionsIterator)->text.c_str(), tmp.x+ 40, tmp.y + 350, 20, 5, { 255,255,255,255 }, 1210);
				//app->render->DrawText((*optionsIterator)->text.c_str(), tmp.x, tmp.y, 30, 5, { 20,20,20,255 });

			}
			else
			app->render->DrawText(font, (*optionsIterator)->text.c_str(), (*optionsIterator)->bounds.x+ 40, (*optionsIterator)->bounds.y + 350, 20, 5, { 255,255,255,255 }, 1210);
			//app->render->DrawText((*optionsIterator)->text.c_str(), (*optionsIterator)->bounds.x, (*optionsIterator)->bounds.y, 30, 5, { 20,20,20,255 });

		}

	}
}

bool Dialogue::CleanUp()
{
	//app->audio->UnLoadFx(talkingFx1);
	//app->audio->UnLoadFx(talkingFx2);
	//app->audio->UnLoadFx(talkingFx3);
	app->audio->UnLoadFx(talkingFx4);

	textToPrint.clear();
	eastl::list<NpcNode*>::iterator it = nodes.begin();

	for (; it != nodes.end(); ++it)
	{
		(*it)->text.clear();

		/*for (eastl::list<DialogueOption*>::iterator item = (*it)->options.begin(); item != (*it)->options.end(); ++item)
		{
			if (*item != nullptr) (*item)->text.clear();
		}
		(*it)->options.clear();*/
	}

	nodes.clear();
	return true;
}

void Dialogue::TriggerDialog(int id)
{
	isDialogueActive = true;
}

void Dialogue::DisableDialog(int id)
{
}

void NpcNode::Reset()
{
	this->dialogFinished = false;
	this->letterCount = 0;
	this->stopDialog = false;
}

DialogueOption* NpcNode::SetCurrentOptionById(int id)
{
	eastl::list<DialogueOption*>::iterator it = options.begin();
	int i = 0;
	while (it != options.end())
	{
		if (id == i)
		{
			currentOption = (*it);
			return (*it);
		}
		++i;
		++it;
	}
	return nullptr;
}