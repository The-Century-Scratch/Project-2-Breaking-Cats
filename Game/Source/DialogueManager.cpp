#include "App.h"
#include "DialogueManager.h"
#include "TextManager.h"
#include "Input.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include <format>

DialogueManager::DialogueManager() : Module()
{
	name = "scene";
}

// Destructor
DialogueManager::~DialogueManager() = default;

// Called before render is available
bool DialogueManager::Awake(pugi::xml_node & config)
{
	LOG("Loading SceneManager");

	
	return true;
}

// Called before the first frame
bool DialogueManager::Start()
{
	uint w, h;
	app.get()->win->GetWindowSize(w, h);
	int x = w;
	int y = h;
	textbox = { x / 2 - 450, y / 2, 900, 200 };
	//progress = GREETING;

	//INTERESTING IF WE WANT TO PUT NAME ON THE PLAYER
	
	//playerName = "CORNELLIUS";
	//firstQuestion.sentenceList->push_front((std::string)"GREETINGS, " + playerName + "! I SEE YOU ARE VERY TIRED FROM YOUR ADVENTURES.");



	//NOTE: Given the size of the font and dialogue box, the maximum length of a sentence is 54 (MAX_CHARACTERS_IN_LINE)

	//MICEY
	M_greeting.sentenceList->push_back("Hello fellow traveller, i see you have entered a store do you wanna buy something?");
	M_answer.sentenceList->push_back("Yes");
	M_answer.sentenceList->push_back("No");
	M_replicationA.sentenceList->push_back("JAJAJA you thought this is only a vetical slice i don't have anything to sell maybe go ask the contrabandist");
	M_replicationB.sentenceList->push_back("Then you are losing your time here");
	
	//CONTRABANDIST
	C_greeting.sentenceList->push_back("Hey you, yes you, do you want some misterious shit?");
	C_answer.sentenceList->push_back("Yes");
	C_answer.sentenceList->push_back("No");
	C_replicationA.sentenceList->push_back("Nah I'm kidding i got nothing this damn devs still haven't gotten me anything. I WANNA SELL ILLEGAL SHIT. Anyway maybe the vendor, Micey has something for you");
	C_replicationB.sentenceList->push_back("Then get the fuck outta here you are acting suspicious");
	C_returnalA.sentenceList->push_back("Already told you I have nothing go ask Micey");
	C_returnalB.sentenceList->push_back("GET OUT!");

	//WARD
	W_conversation.sentenceList->push_back("Hello traveller, what do you want");
	W_conversation.sentenceList->push_back("Hello, why can't I go through that way");
	W_conversation.sentenceList->push_back("Sorry this is only a vertical slice you would fall in an eternal hole so you just can't go");
	W_returnal.sentenceList->push_back("Dude already told you that you can't cross, so annoying this kid");

	//SIGN
	S_text.sentenceList->push_back("IDK WHAT ELSE TO PUT HERE");

	micey = M_GREETING;
	contrabandist = C_GREETING;
	ward = W_CONVERSATION;

	

	//importnat to start any dialog
	//sentenceQueue = firstQuestion.sentenceList;

	return true;
}

// Called each loop iteration
bool DialogueManager::PreUpdate()
{
	return true;
}


bool DialogueManager::Pause(int phase)
{
	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->LoadGameRequest();

	return true;
}

// Called each loop iteration
bool DialogueManager::Update(float dt)
{
	if (dialogueEnabled) {
		//TODO 2: Draw the Textbox and the Text in front of it
		DrawTextbox();
		DrawDialogText(sentenceQueue->front(), 0);
		//
	}

	if (questionEnabled) {
		//TODO 2: Draw the Text of option 1 and 2
		
		auto second_elem = M_answer.sentenceList->begin();
		std::advance(second_elem, 1);

		DrawDialogText(M_answer.sentenceList->front(), 80);
		DrawDialogText(*second_elem, 100);

		//TODO 3: Detect if 1 or 2 are being pressed, and for each case, set the progress stage we are in and update the dialogue
		if (app->input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN) {
			if (miceyDialog) {
				micey = M_REPLICATION1;
			}
			else {
				contrabandist = C_REPLICATION1;
			}

			UpdateDialogue();
			questionEnabled = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_X) == KeyState::KEY_DOWN) {
			if (miceyDialog) {
				micey = M_REPLICATION2;
			}
			else {
				contrabandist = C_REPLICATION2;
			}
			UpdateDialogue();
			questionEnabled = false;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && dialogueEnabled) {
		UpdateDialogue();
	}

	return true;
}

// Called each loop iteration
bool DialogueManager::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
		return false;

	return true;
}


bool DialogueManager::DrawTextbox() {
	//app->render->DrawRectangle({ textbox.x - 10, textbox.y - 10, textbox.w + 20, textbox.h + 20 }, 255, 255, 255, 255, true, false);
	//app->render->DrawRectangle(textbox, 50, 50, 50, 255, true, false);

	return true;
}

bool DialogueManager::DrawDialogText(std::string s, uint yoffset) {
	uint string_size = s.size();
	for (int i = 0; i <= string_size; i += MAX_CHARACTERS_IN_LINE) {
		if (s.size() > MAX_CHARACTERS_IN_LINE) {
			std::string s1 = s;
			for (int i = MAX_CHARACTERS_IN_LINE - 1; i >= 0; i--) {
				if (s[i] == ' ') {
					s = s.substr(i + 1, s.size());
					s1 = s1.substr(0, i);
					break;
				}
			}
			//app->fonts->BlitText(textbox.x + 20, textbox.y + 20 + 20 * (i / MAX_CHARACTERS_IN_LINE) + yoffset, fontId, s1.c_str(), false;
			TextParameters params(0, DrawParameters(0, iPoint(textbox.x + 20, textbox.y + 20 + 20 * (i / MAX_CHARACTERS_IN_LINE) + yoffset)));
			params.align = AlignTo::ALIGN_TOP_LEFT;
			app->fonts->DrawText(
				s1.c_str(),
				params
			);
		}
		else {
			//app->fonts->BlitText(textbox.x + 20, textbox.y + 20 + 20 * (i / MAX_CHARACTERS_IN_LINE) + yoffset, fontId, s.c_str(), false);
			TextParameters params(0, DrawParameters(0, iPoint(textbox.x + 20, textbox.y + 20 + 20 * (i / MAX_CHARACTERS_IN_LINE) + yoffset)));
			params.align = AlignTo::ALIGN_TOP_LEFT;
			app->fonts->DrawText(
				s.c_str(),
				params
			);
		}
	}

	return true;

}

bool DialogueManager::SetDialogue(Dialogue dialogue) {

	sentenceQueue = dialogue.sentenceList;

	return true;
}

bool DialogueManager::UpdateDialogue() {
	if (miceyDialog == true) {
		switch (micey) {
		case M_GREETING:
			SetDialogue(M_greeting);
			questionEnabled = true;
			break;
		case M_REPLICATION1:
			SetDialogue(M_replicationA);
			questionEnabled = false;
			micey = M_END;
			break;
		case M_REPLICATION2:
			SetDialogue(M_replicationB);
			questionEnabled = false;
			micey = M_END;
			break;
		case M_END:
			dialogueEnabled = false;
			miceyDialog = false;
			break;
		}
	}

	if (contrabandistDialog == true) {
		switch (contrabandist) {
		case C_GREETING:
			SetDialogue(C_greeting);
			questionEnabled = true;
			break;
		case C_REPLICATION1:
			SetDialogue(C_replicationA);
			questionEnabled = false;
			contrabandist = C_RETURNAL1;
			break;
		case C_REPLICATION2:
			SetDialogue(C_replicationB);
			questionEnabled = false;
			contrabandist = C_RETURNAL2;
			break;
		case C_RETURNAL1:
			SetDialogue(C_returnalA);
			dialogueEnabled = false;
			break;
		case C_RETURNAL2:
			SetDialogue(C_returnalB);
			dialogueEnabled = false;
			break;
		}
	}
	
	if (wardDialog == true) {
		switch (ward) {
		case W_CONVERSATION:
			SetDialogue(W_conversation);
			sentenceQueue->pop_front();
			questionEnabled = false;
			if (sentenceQueue->size() == 1) {
				ward = W_RETURNAL;
			}
			break;
		case W_RETURNAL:
			SetDialogue(W_returnal);
			dialogueEnabled = false;
			break;
		}
	}

	if (signDialog == true) {
		SetDialogue(S_text);
		dialogueEnabled = false;
	}

	return true;
}

bool DialogueManager::ResetDialogue() {
	micey = M_GREETING;
	contrabandist = C_GREETING;
	ward = W_CONVERSATION;

	//sentenceQueue = firstQuestion.sentenceList;
	questionEnabled = false;
	dialogueEnabled = false;

	return true;
}

// Called before quitting
bool DialogueManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool DialogueManager::HasSaveData() const
{
	return true;
}

bool DialogueManager::LoadState(pugi::xml_node const& data)
{
	return false;
}

pugi::xml_node DialogueManager::SaveState(pugi::xml_node const& data) const
{
	std::string saveData2 = "<{} {}=\"{}\"/>\n";
	std::string saveOpenData2 = "<{} {}=\"{}\">\n";
	std::string saveData4 = "<{} {}=\"{}\" {}=\"{}\"/>\n";
	std::string saveOpenData4 = "<{} {}=\"{}\" {}=\"{}\">\n";
	std::string saveData6 = "<{} {}=\"{}\" {}=\"{}\" {}=\"{}\"/>\n";
	std::string saveData6OneFloat = "<{} {}=\"{}\" {}=\"{}\" {}=\"{}\" {}=\"{:.2f}\"/>\n";
	std::string saveData6OneInt = "<{} {}=\"{}\" {}=\"{:.2f}\" {}=\"{:.2f}\" {}=\"{:.2f}\"/>\n";
	std::string saveFloatData = "<{} {}=\"{:.2f}\" {}=\"{:.2f}\"/>\n";
	std::string dataToSave = "<scene>\n";
	dataToSave += "</scene>";

	app->AppendFragment(data, dataToSave.c_str());

	return data;
}
