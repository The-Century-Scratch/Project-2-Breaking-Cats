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
	progress = GREETING;

	playerName = "CORNELLIUS";

	//NOTE: Given the size of the font and dialogue box, the maximum length of a sentence is 54 (MAX_CHARACTERS_IN_LINE)

	//TODO 5: Substitute "Traveler" by "playerName"
	firstQuestion.sentenceList->push_front((std::string)"GREETINGS, " + playerName + "! I SEE YOU ARE VERY TIRED FROM YOUR ADVENTURES.");
	//
	firstQuestion.sentenceList->push_front("MAY I INTEREST YOU IN A GLASS OF MILK, COOKIES AND A BALL OF YOUR FAVORITE ICE CREAM FLAVOR? OR PERHAPS YOU WOULD RATHER GO TO BED ALREADY, SIR?");

	firstOption.sentenceList->push_front("/PRESS 1/ FOOD");
	firstOption.sentenceList->push_front("/PRESS 2/ BED");

	answerA.sentenceList->push_front("PERFECT! I WILL PREPARE YOUR FOOD RIGHT AWAY, SIR.");

	answerB.sentenceList->push_front("GREAT! LET ME GET THE KEY TO YOUR ROOM AND YOU WILL BE ALL SET, SIR.");

	firstQuestionA.sentenceList->push_front(playerName + ", SIR, PLEASE ALLOW ME	SOME TIME TO PREPARE THE DISHES!");
	firstQuestionA.sentenceList->push_front("THERE IS A STOOL OVER THERER FOR YOU TO WAIT ON.");

	firstQuestionB.sentenceList->push_front("IN A MINUTE, SIR!");
	firstQuestionB.sentenceList->push_front("THE PREVIOUS HOST LEFT RATHER QUICKLY, I DO NOT KNOW WHERE THE MAN LEFT THOSE DARN KEYS!");

	//fontId = app->fonts->Load("Assets/Fonts/defaultfont.png", "!,_./0123456789;?ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 1);

	sentenceQueue = firstQuestion.sentenceList;

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
		auto second_elem = firstOption.sentenceList->begin();
		std::advance(second_elem, 1);

		DrawDialogText(firstOption.sentenceList->front(), 80);
		DrawDialogText(*second_elem, 100);
		//

		//TODO 3: Detect if 1 or 2 are being pressed, and for each case, set the progress stage we are in and update the dialogue
		if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) {
			progress = ANSWERA;
			UpdateDialogue();
		}

		if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN) {
			progress = ANSWERB;
			UpdateDialogue();
		}
		//
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
	switch (progress) {
		//TODO 4: Write each case 
	case GREETING:
		questionEnabled = true;
		break;
	case ANSWERA:
		SetDialogue(answerA);
		questionEnabled = false;
		progress = GREETINGA;
		break;
	case ANSWERB:
		SetDialogue(answerB);
		questionEnabled = false;
		progress = GREETINGB;
		break;
	case GREETINGA:
		SetDialogue(firstQuestionA);
		dialogueEnabled = false;
		break;
	case GREETINGB:
		SetDialogue(firstQuestionB);
		dialogueEnabled = false;
		break;		
	}

	return true;
}

bool DialogueManager::ResetDialogue() {
	progress = GREETING;
	sentenceQueue = firstQuestion.sentenceList;
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
