#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "QuestManager.h"
#include "Window.h"
#include "Debug.h"
#include "Inventory.h"
#include "SceneManager.h"
#include "SceneGameplay.h"
#include "GuiManager.h"
#include "StatsManager.h"
#include "Hud.h"

//#include "Font.h"
#include "Item.h"
//#include "UltraPotion.h"
//#include "Potion.h"
//#include "OmniPotion.h"
//#include "FairyTear.h"
//#include "Orb.h"
//#include "OrbFragment.h"
//#include "KnightHelmet.h"
//#include "KnightChest.h"

#include "Easings.h"

#define SLOT_OFFSET 160

Inventory::Inventory(bool startEnabled) : Module(startEnabled)
{
	name.Create("Inventory");
	/*players = pls;
	atlasTexture = atlas;

	easing = new Easing(true, 0, -200, 350, 90);
	easing2 = new Easing(false, 0, 150, 1150, 90);*/
}

Inventory::~Inventory()
{
}

bool Inventory::Start()
{
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	itemTexture =	app->tex->Load(config.child("itemTexture").attribute("texturepath").as_string());
	invTex =		app->tex->Load(config.child("invTexture").attribute("texturepath").as_string());
	slotText =		app->tex->Load(config.child("slotTexture").attribute("texturepath").as_string());
	gatsText =		app->tex->Load(config.child("gatsTexture").attribute("texturepath").as_string());
	catskaText =	app->tex->Load(config.child("catskaTexture").attribute("texturepath").as_string());
	itemMenuTexture =	app->tex->Load(config.child("itemMenuTexture").attribute("texturepath").as_string());


	isActivated = false;
	isItemMenu_Active = false;

	showStatsId = 1;

	slotRect =				{ 0,  0, 36, 36 };
	slotRectFocus =			{ 36, 0, 36, 36 };

	itemMenuBounds =		{ 0, 0,	  104, 115 };
	itemMenuPos =			{ 0, 0,   0,   0   };
	itemMenuButton =		{ 0, 115, 104, 25  };
	itemMenuButtonFocus =	{ 0, 140, 104, 25  };
	itemMenuButtonPos =		{ 0, 0,   0,   0   };

	gatsRect =				{ 0, 0, 32,32 };
	catskaRect =			{ 0, 0, 32,32 };

	leftArrowRect =			{ 72, 16, 22, 16 };
	leftArrowRectFocus =	{ 94, 16, 22, 16 };
	rightArrowRect =		{ 72, 0,  22, 16 };
	rightArrowRectFocus =	{ 94, 0,  22, 16 };

	boundsLeftArrow =		{ 0, 0, 22, 16 };
	boundsRightArrow =		{ 0, 0, 22, 16 };


	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		slotList[invSlot_].bounds = { 0, 0, 36, 36 };
		slotList[invSlot_].currentSlot = invSlot_;
		slotList[invSlot_].itemId = 0;
		slotList[invSlot_].isfull = false;
		slotList[invSlot_].state = SlotState::UNSELECTED;
	}

	easing = new Easing(false, 0, -40, 40, 100);

	/*InventorySlot* newInvSlot_ = nullptr;
	for (size_t maxInvSlots_ = 1; maxInvSlots_ <= MAX_INVENTORY_SLOTS; maxInvSlots_++)
	{
		newInvSlot_ = new InventorySlot();
 		newInvSlot_->bounds = { 0, 0, 16, 16 };
		newInvSlot_->currentSlot = maxInvSlots_;
		newInvSlot_->itemId = 0;
		newInvSlot_->isfull = false;
		newInvSlot_->state = SlotState::UNSELECTED;
		slotList.Add(newInvSlot_);
	}*/

	//guiTex = app->tex->Load("Textures/UI/gui_inventory.png");
	//buttonTex = app->tex->Load("Textures/UI/gui_pad_buttons.png");

	//btnEquipment = new GuiButton(1, { -200,116,274,78 }, "Equipment", this, font);
	//btnEquipment->alineation = 0;
	//btnEquipment->texture = guiTex;
	//btnEquipment->section = { 739,0,274,78 };

	//btnItems = new GuiButton(2, { -200,204,274,78 }, "Items", this, font);
	//btnItems->alineation = 0;
	//btnItems->texture = guiTex;
	//btnItems->section = { 739,0,274,78 };

	///*btnInfo = new GuiButton(3, { 150,292,274,78 }, "Stats", this, font);
	//btnInfo->alineation = 0;
	//btnInfo->texture = guiTex;
	//btnInfo->section = { 739,0,274,78 };*/
	//
	///*btnHunter = new GuiButton(6, { 0,0, 40,40 }, "", this, font);
	//btnHunter->texture = atlasTexture;
	//btnHunter->section = { 0,722,32,32 };

	//btnWizard = new GuiButton(7, { 0,0, 40,40 }, "", this, font);
	//btnWizard->texture = atlasTexture;
	//btnWizard->section = { 35,722,32,32 };

	//btnThief = new GuiButton(8, { 0,0, 40,40 }, "", this, font);
	//btnThief->texture = atlasTexture;
	//btnThief->section = { 71,722,32,32 };

	//btnWarrior = new GuiButton(9, { 0,0, 40,40 }, "", this, font);
	//btnWarrior->texture = atlasTexture;
	//btnWarrior->section = { 103,722,25,32 };*/

	//// Next and back character buttons
	//btnNext = new GuiButton(10, { 295, 149, 46, 124 }, "", this, font);
	//btnNext->texture = guiTex;
	//btnNext->section = { 890, 219, 46, 124 };
	//btnNext->sectionFocused = { 890,469,46,124 };

	//btnPrev = new GuiButton(11, { 119, 149, 46, 124 }, "", this, font);
	//btnPrev->texture = guiTex;
	//btnPrev->section = { 890, 90, 46, 124 };
	//btnPrev->sectionFocused = { 890, 345, 46, 124 };

	//btnUse = new GuiButton(4, { 0,0,128,32 }, "USE", this, font);
	//btnUse->texture = nullptr;
	//btnUse->alineation = 1;

	//btnDelete = new GuiButton(5, { 0,0,128,32 }, "DEL", this, font);
	//btnDelete->texture = nullptr;
	//btnDelete->alineation = 1;

	//btnUnEquip = new GuiButton(12, { 0,0,128, 32 }, "UNEQUIP", this, font);
	//btnUnEquip->texture = nullptr;
	//btnUnEquip->alineation = 1;

	////btnNextStats = new GuiButton(13, {}, "", this, font);
	////btnPrevStats = new GuiButton(14, {}, "", this, font);

	//controls.push_back(btnEquipment);
	//controls.push_back(btnItems);
	////buttons.push_back(btnInfo);

	//// Initialize items;
	//int offsetX = 0;
	//int offsetY = 0;

	//for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	//{
	//	slots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
	//	slots[i].id = i;
	//	slots[i].filled = false;
	//	slots[i].state = SlotState::NONE;
	//	slots[i].item = nullptr;
	//	slots[i].itemsAmount = 0;

	//	armorSlots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
	//	armorSlots[i].id = i;
	//	armorSlots[i].filled = false;
	//	armorSlots[i].state = SlotState::NONE;
	//	armorSlots[i].item = nullptr;
	//	armorSlots[i].itemsAmount = 0;

	//	offsetX += slots[i].bounds.w + 7;
	//	if (i == 7 || i == 15 || i == 23)
	//	{
	//		offsetY += slots[i].bounds.h + 7;
	//		offsetX = 0;
	//	}
	//}

	//currentPlayer = (*players.begin());

	//for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
	//{
	//	equipment[i].bounds = {755, 136 + (i * 55) , 40, 40};
	//	equipment[i].id = i;
	//	equipment[i].filled = false;
	//	equipment[i].state = SlotState::NONE;
	//	equipment[i].item = nullptr;
	//	equipment[i].itemsAmount = 0;
	//}

	//armorSlots->filled = false;
	//armorSlots->itemsAmount = 0;

	//slots->itemsAmount = 0;
	//slots->filled = false;

	//state = InventoryState::NONE;
	//grabbed = false;
	//toGrabCount = 0.0f;
	//originSlot = nullptr;
	//isTextDisplayed = false;
	//displayEquipmentMenu = false;

	//currentSlotId = 0;
	//currentArmorSlotId = 2;
	//inEquipment = false;

	//usingItem = false;

	//currentControl = nullptr;
	//lastControl = nullptr;

	//lastUserInput = 0;

	return true;
}

bool Inventory::Update(float dt)
{
	if (isActivated)
	{
		if (easing->easingsActivated)
		{
			easingPos = easing->bounceEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
			if (easing->currentIteration < easing->totalIterations)
			{
				easing->currentIteration++;
			}
			else
			{
				easing->currentIteration = 0;
				easing->easingsActivated = false;
			}
		}

		int scale = app->win->GetScale();

		invPos.x = (-app->render->camera.x / scale) + 25;
		invPos.y = (-app->render->camera.y / scale) + easingPos;

		invPosText.x = (int)(app->render->camera.x + invPos.x * scale);
		invPosText.y = (int)(app->render->camera.y + invPos.y * scale) + easingPos;

		iPoint invSpacing = { 0,0 };
		for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
		{
			slotList[invSlot_].bounds.x = invPos.x + invSpacing.x + 193;
			slotList[invSlot_].bounds.y = invPos.y + invSpacing.y + 63;
			invSpacing.x += 39;
			if (invSlot_ == 3 || invSlot_ == 7 || invSlot_ == 11) invSpacing = { 0, invSpacing.y += 39 };
		}

		boundsLeftArrow.x = invPos.x + 20;
		boundsLeftArrow.y = invPos.y + 93;
		boundsRightArrow.x = invPos.x + 154;
		boundsRightArrow.y = invPos.y + 93;

		HandleStatsInfo();

		HandleSlotState();

		if (isItemMenu_Active)
		{
			ItemMenu(currentSlot_);
		}

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
			SaveMouseClickPos();
			OnMouseClickEvent();
		}

	}

	return true;
}

bool Inventory:: PostUpdate()
{
	//Add items --> the final idea is to create a switch wich gets an id from 1-6 from the quest or chest and the switch will add the corresponding item to the inv
	if (app->input->GetKey(SDL_SCANCODE_I) == KeyState::KEY_DOWN && app->hud->hudstate == hudSTATE::CLOSED && !app->questManager->printQuestMenu)
	{
		app->inventory->isActivated = !app->inventory->isActivated;
		app->sceneManager->Pause = app->inventory->isActivated;
		app->audio->PlayFx(app->hud->swapscenesfx);
		SDL_ShowCursor(SDL_ENABLE);
	}
	if (CONTROLLERX && app->hud->hudstate == hudSTATE::CLOSED && !app->questManager->printQuestMenu)
	{
		app->inventory->isActivated = !app->inventory->isActivated;
		app->sceneManager->Pause = app->inventory->isActivated;
		app->audio->PlayFx(app->hud->swapscenesfx);
		SDL_ShowCursor(SDL_DISABLE);
	}

	if (isActivated)
	{
		Draw();
	}

	return true;
}

void Inventory::ItemMenu(int curSlot_)
{
	itemMenuButtonPos.x = slotList[curSlot_].bounds.x - itemMenuBounds.w;
	itemMenuButtonPos.y = slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 88;
	itemMenuButtonPos.w = itemMenuButton.w;
	itemMenuButtonPos.h = itemMenuButton.h;

	itemMenuPos.x = slotList[curSlot_].bounds.x - itemMenuBounds.w;
	itemMenuPos.y = slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5;
	itemMenuPos.w = itemMenuBounds.w;
	itemMenuPos.h = itemMenuBounds.h;

	if (IsMouseInside(itemMenuButtonPos) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		slotList[curSlot_].item->equiped = !slotList[curSlot_].item->equiped;
	}
}

void Inventory::ItemMenuDraw(int curSlot_)
{
	int scale = app->win->GetScale();
	SDL_Color white = { 255, 255, 255, 255 };
	app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5, &itemMenuBounds);

	if (IsMouseInside(itemMenuButtonPos))
	{
		app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 88, &itemMenuButtonFocus);
	}
	else
	{
		app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 88, &itemMenuButton);
	}

	if (slotList[curSlot_].item->equiped)
	{
		app->render->DrawText("UNEQUIP", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 5) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 91) * scale, 94 * scale, 19 * scale, white);
	}
	else
	{
		app->render->DrawText(" EQUIP ", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 5) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 91) * scale, 94 * scale, 19 * scale, white);
	}
	SString pep = "te mare tenia una polla\n que ja la voldria jo";

	switch (slotList[curSlot_].itemId)
	{
	case 1:
		app->render->DrawText("Fire Paws", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 12) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 80 * scale, 14 * scale, white);
		app->render->DrawText("Dash  through  enemies", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("and  damage  them", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	case 2:
		app->render->DrawText("Dragon Slayer", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("Heal  when  killing", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("and  gain  speed", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	case 3:
		app->render->DrawText("Grappling Hook", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("When  hit  by  an", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("enemy  move  towards", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 52) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("the  nearest  ally", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 65) * scale, 84 * scale, 10 * scale, white);
		break;
	case 4:
		app->render->DrawText("Penetration Bullet", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("Attacks  pass ", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("through  enemies", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	case 5:
		app->render->DrawText("Mystical Energy", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("immortal  2  turns", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("when  teleporting", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		
		break;
	case 6:
		app->render->DrawText("Arcane  Spirit", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("Gain  the  ability", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("to  teleport", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	default:
		break;
	}
}

int Inventory::GetItemEquipped()
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (slotList[invSlot_].item->equiped)
		{
			return slotList[invSlot_].itemId;
		}
	}
	return 0;
}

Player* Inventory::GetPlayers(ListItem<Player*>* pl_)
{
	return currentPlayer;
}

int Inventory::GetFirePaw() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 1)
		{
			return i.itemId;
		}
	}
	return 0;
}

int Inventory::GetDragonSlayer() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 2)
		{
			return i.itemId;
		}
	}
	return 0;
}


int Inventory::GetGrapplingHook() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 3)
		{
			return i.itemId;
		}
	}
	return 0;
}

int Inventory::GetBulletPenetration() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 4)
		{
			return i.itemId;
		}
	}
	return 0;
}

int Inventory::GetMysticalEnergy() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 5)
		{
			return i.itemId;
		}
	}
	return 0;
}

int Inventory::GetArcaneSpirit() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
{
	for (auto& i : slotList)
	{
		if (i.itemId == 6)
		{
			return i.itemId;
		}
	}
	return 0;
}

void Inventory::Draw()
{
	int scale = app->win->GetScale();
	SDL_Color white = {255, 255, 255, 255};
	SDL_Rect invRect = { 0, 0, 372, 240 };
	app->render->DrawTexture(invTex, invPos.x, invPos.y, &invRect);
	app->render->DrawText("INVENTORY",invPosText.x + 222 * scale, invPosText.y + 27 * scale, 92 * scale, 24 * scale, white);
	app->render->DrawText("STATS", invPosText.x + 77 * scale, invPosText.y + 127 * scale, 38 * scale, 11 * scale, white);

	if (IsMouseInside(boundsLeftArrow))
	{
		app->render->DrawTexture(slotText, boundsLeftArrow.x, boundsLeftArrow.y, &leftArrowRectFocus);
	}
	else
	{
		app->render->DrawTexture(slotText, boundsLeftArrow.x, boundsLeftArrow.y, &leftArrowRect);
	}

	if (IsMouseInside(boundsRightArrow))
	{
		app->render->DrawTexture(slotText, boundsRightArrow.x, boundsRightArrow.y, &rightArrowRectFocus);
	}
	else
	{
		app->render->DrawTexture(slotText, boundsRightArrow.x, boundsRightArrow.y, &rightArrowRect);
	}

	app->render->DrawText("HP ", invPosText.x + 48 * scale, invPosText.y + 156 * scale, 30 * scale, 18 * scale, white);
	app->render->DrawText("DMG", invPosText.x + 48 * scale, invPosText.y + 186 * scale, 30 * scale, 18 * scale, white);

	switch (showStatsId)
	{
	case 1:
		app->render->DrawTexture(gatsText, invPos.x + 82, invPos.y + 33, &gatsRect);
		app->render->DrawText("GATS", invPosText.x + 74 * scale, invPosText.y + 95 * scale, 50 * scale, 12 * scale, white);
		app->render->DrawText(std::to_string(app->statsManager->gats_hp).c_str(), invPosText.x + 100 * scale, invPosText.y + 156 * scale, 30 * scale, 18 * scale, white);
		app->render->DrawText(std::to_string(app->statsManager->gats_dmg).c_str(), invPosText.x + 100 * scale, invPosText.y + 186 * scale, 30 * scale, 18 * scale, white);
		break;
	case 2:
		app->render->DrawTexture(catskaText, invPos.x + 82, invPos.y + 33, &catskaRect);
		app->render->DrawText("CATSKA", invPosText.x + 70 * scale, invPosText.y + 95 * scale, 60 * scale, 12 * scale, white);
		app->render->DrawText(std::to_string(app->statsManager->catska_hp).c_str(), invPosText.x + 100 * scale, invPosText.y + 156 * scale, 30 * scale, 18 * scale, white);
		app->render->DrawText(std::to_string(app->statsManager->catska_dmg).c_str(), invPosText.x + 100 * scale, invPosText.y + 186 * scale, 30 * scale, 18 * scale, white);
		break;
	default:
		break;
	}
	app->render->DrawText("HP ", invPosText.x + 48 * scale, invPosText.y + 156 * scale, 30 * scale, 18 * scale, white);
	//app->render->DrawText(std::to_string(MouseX_).c_str(), app->debug->debugX + 110, app->debug->debugY + 160, 50, 20, app->debug->debugColor);
	app->render->DrawText("DMG", invPosText.x + 48 * scale, invPosText.y + 186 * scale, 30 * scale, 18 * scale, white);
	//app->render->DrawText(std::to_string(MouseY_).c_str(), app->debug->debugX + 110, app->debug->debugY + 190, 50, 20, app->debug->debugColor);

	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		app->render->DrawTexture(slotText, slotList[invSlot_].bounds.x, slotList[invSlot_].bounds.y, &slotRect);

		switch (slotList[invSlot_].state)
		{
		case SlotState::UNSELECTED:
			break;
		case SlotState::FOCUSED:
			app->render->DrawTexture(slotText, slotList[invSlot_].bounds.x, slotList[invSlot_].bounds.y, &slotRectFocus);
			break;
		case SlotState::SELECTED:
			break;
		default:
			break;
		}

		if (slotList->itemId != 0)
		{
			app->render->DrawTexture(itemTexture, slotList[invSlot_].bounds.x+2, slotList[invSlot_].bounds.y+2, &slotList[invSlot_].itemTextureBounds);
		}

		if (isItemMenu_Active)
		{
			ItemMenuDraw(currentSlot_);
		}

	}
}

bool Inventory::CleanUp()
{
	//RELEASE(btnEquipment);
	//RELEASE(btnItems);
	//RELEASE(btnUse);
	//RELEASE(btnDelete);
	///*RELEASE(btnHunter);
	//RELEASE(btnWizard);
	//RELEASE(btnThief);
	//RELEASE(btnWarrior);*/
	//RELEASE(btnUnEquip);
	//RELEASE(btnNext);
	//RELEASE(btnPrev);

	//RELEASE(easing);
	app->tex->Unload(invTex);
	app->tex->Unload(slotText);
	app->tex->Unload(itemTexture);
	app->tex->Unload(gatsText);
	app->tex->Unload(catskaText);


	/*for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		delete(slotList);
	}*/
	
	////RELEASE(atlasTexture);
	//app->tex->UnLoad(guiTex);

	//controls.clear();
	//players.clear();

	return true;
}

bool Inventory::OnMouseClickEvent()
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (IsMouseClickInside(itemMenuPos, mouseClickPos))
		{
			break;
		}
		if (IsMouseClickInside(slotList[invSlot_].bounds, mouseClickPos))
		{
			if (slotList[invSlot_].itemId != 0)
				isItemMenu_Active = true;

			currentSlot_ = slotList[invSlot_].currentSlot;
			break;
		}
		else if(!IsMouseClickInside(slotList[invSlot_].bounds, mouseClickPos))
		{
			isItemMenu_Active = false;
			itemMenuPos = { 0, 0, 0, 0 };
		}
	}

	return false;
}

void Inventory::UpdatingButtons(Input* input)
{
	//int prevX = xMouse;
	//int prevY = yMouse;
	//input->GetMousePosition(xMouse, yMouse);
	//if (prevX != xMouse || prevY != yMouse)
	//{
	//	lastUserInput = 1;
	//	SDL_ShowCursor(SDL_ENABLE);
	//}
	//else
	//{
	//	lastUserInput = 0;
	//}

	//if (state == InventoryState::EQUIPMENT || state == InventoryState::ITEMS)
	//{
	//	if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_REPEAT)
	//	{
	//		currentControl = btnPrev;
	//		btnPrev->state = GuiControlState::PRESSED;
	//	}
	//	else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
	//	{
	//		currentControl = btnNext;
	//		btnNext->state = GuiControlState::PRESSED;
	//	}

	//	if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_UP)
	//	{
	//		btnPrev->state = GuiControlState::NORMAL;
	//		app->audio->PlayFx(btnPrev->channel, btnPrev->clickFx);
	//		btnPrev->isPlayable = true;
	//		btnPrev->NotifyObserver();
	//		/*currentControl = btnPrev;*/
	//	}
	//	else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_UP)
	//	{
	//		btnNext->state = GuiControlState::NORMAL;
	//		app->audio->PlayFx(btnNext->channel, btnNext->clickFx);
	//		btnNext->isPlayable = true;
	//		btnNext->NotifyObserver();
	//		/*currentControl = btnNext;*/
	//	}

	//}

	//if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	//{
	//	if (currentControl != btnPrev && currentControl != btnNext)
	//	{
	//		if (currentControl == nullptr)
	//		{
	//			currentControl = (*controls.begin());
	//			SDL_ShowCursor(SDL_DISABLE);
	//		}
	//		else
	//		{
	//			eastl::list<GuiControl*>::iterator it = controls.begin();
	//			for (int i = 0; i < controls.size(); ++i, ++it)
	//			{
	//				if ((*it)->id == currentControl->id + 1)
	//				{
	//					currentControl = (*it);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	//{
	//	if (currentControl != btnPrev && currentControl != btnNext)
	//	{
	//		if (currentControl == nullptr)
	//		{
	//			currentControl = (*controls.begin());
	//			SDL_ShowCursor(SDL_DISABLE);
	//		}
	//		else
	//		{
	//			eastl::list<GuiControl*>::iterator it = controls.begin();
	//			for (int i = 0; i < controls.size(); ++i, ++it)
	//			{
	//				if ((*it)->id == currentControl->id - 1)
	//				{
	//					currentControl = (*it);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
}

bool Inventory::LoadState(pugi::xml_node& node)
{
	/*pugi::xml_node n = node.child("inventory").child("items");
	int i;
	for (i = 0, n = n.child("slot"); n && i < MAX_INVENTORY_SLOTS; n = n.next_sibling("slot"), ++i)
	{
		Item* item = nullptr;
		pugi::xml_node it = n.child("item");
		if ((ObjectType)it.child("object_type").attribute("value").as_int() == ObjectType::ITEM)
		{
			switch ((ItemType)it.child("item_type").attribute("value").as_int())
			{
			case ItemType::POTION:
				item = new Potion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::ULTRA_POTION:
				item = new UltraPotion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::OMNI_POTION:
				item = new OmniPotion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::FAIRY_TEAR:
				item = new FairyTear(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			}
		}

		if (slots[i].item != nullptr)
		{
			slots[i].item->UnLoad();
			RELEASE(slots[i].item);
			slots[i].itemsAmount = 0;
			slots[i].filled = false;
			slots[i].state = SlotState::NONE;
		}
		if (item != nullptr)
		{
			item->Load();
			slots[i].item = item;
			slots[i].id = i;
			slots[i].itemsAmount = n.attribute("amount").as_int();
			slots[i].filled = n.attribute("filled").as_bool();
			slots[i].state = (SlotState)n.attribute("state").as_int();
		}
	}

	n = node.child("inventory").child("armor");
	i = 0;
	for (n = n.child("slot"); n && i < MAX_INVENTORY_SLOTS; n = n.next_sibling("slot"), ++i)
	{
		Item* item = nullptr;
		SDL_Rect r;
		pugi::xml_node it = n.child("item");
		if ((ObjectType)it.child("object_type").attribute("value").as_int() == ObjectType::ARMOR)
		{
			switch ((ArmorType)it.child("armor_type").attribute("value").as_int())
			{
			case ArmorType::HELMET:
				r = { it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int(), it.child("position").attribute("w").as_int(), it.child("position").attribute("h").as_int() };
				item = new KnightHelmet(r, iPoint(r.x, r.y), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ArmorType::CHEST:
				r = { it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int(), it.child("position").attribute("w").as_int(), it.child("position").attribute("h").as_int() };
				item = new KnightChest(r, iPoint(r.x, r.y), atlasTexture, it.attribute("map_name").as_string());
				break;
			}
		}

		if (armorSlots[i].item != nullptr)
		{
			armorSlots[i].item->UnLoad();
			RELEASE(slots[i].item);
			armorSlots[i].itemsAmount = 0;
			armorSlots[i].filled = false;
			armorSlots[i].state = SlotState::NONE;
		}
		if (item != nullptr)
		{
			item->Load();
			armorSlots[i].item = item;
			armorSlots[i].id = i;
			armorSlots[i].itemsAmount = n.attribute("amount").as_int();
			armorSlots[i].filled = n.attribute("filled").as_bool();
			armorSlots[i].state = (SlotState)n.attribute("state").as_int();
		}
	}*/
	return true;
}

bool Inventory::SaveState(pugi::xml_node& node)
{
	//pugi::xml_node n = node.append_child("inventory");
	//pugi::xml_node it = n.append_child("items");
	//for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	//{
	//	// Items
	//	pugi::xml_node item = it.append_child("slot");
	//	if (slots[i].filled)
	//	{
	//		item.append_attribute("amount").set_value(slots[i].itemsAmount);
	//		item.append_attribute("filled").set_value(slots[i].filled);
	//		item.append_attribute("id").set_value(slots[i].id);
	//		item.append_attribute("state").set_value((int)slots[i].state);
	//		pugi::xml_node it = item.append_child("item");
	//		if(slots[i].item != nullptr)
	//			slots[i].item->SaveState(it);
	//	}
	//}

	//it = n.append_child("armor");
	//for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	//{
	//	// Armor
	//	pugi::xml_node item = it.append_child("slot");
	//	if (armorSlots[i].filled)
	//	{
	//		item.append_attribute("amount").set_value(armorSlots[i].itemsAmount);
	//		item.append_attribute("filled").set_value(armorSlots[i].filled);
	//		item.append_attribute("id").set_value(armorSlots[i].id);
	//		item.append_attribute("state").set_value((int)armorSlots[i].state);
	//		pugi::xml_node it = item.append_child("item");
	//		armorSlots[i].item->SaveState(it);
	//	}
	//}
	return true;
}

void Inventory::AddItem(Item *it)
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (slotList[invSlot_].itemId == 0)
		{
			slotList[invSlot_].itemId = it->itemid;
			slotList[invSlot_].itemTextureBounds = it->itemTextSection;
			slotList[invSlot_].item = it;
			break;
		}
	}
}

bool Inventory::IsMouseInside(SDL_Rect r)
 {
	SDL_Rect auxR = {r.x-invPos.x+25, r.y-invPos.y, r.w, r.h};

	int x, y;
	app->input->GetMousePosition(x, y); 

	return (x > auxR.x) && (x < auxR.x + auxR.w) && (y > auxR.y) && (y < auxR.y + auxR.h);
}

bool Inventory::IsMouseClickInside(SDL_Rect r, iPoint clickPos)
{
	SDL_Rect auxR2 = { r.x - invPos.x + 25, r.y - invPos.y, r.w, r.h };

	return (clickPos.x > auxR2.x) && (clickPos.x < auxR2.x + auxR2.w) && (clickPos.y > auxR2.y) && (clickPos.y < auxR2.y + auxR2.h);
}

void Inventory::SaveMouseClickPos()
{
	app->input->GetMousePosition(mouseClickPos.x, mouseClickPos.y);
}

void Inventory::DisplayText(SDL_Rect bounds, bool showColliders)
{
	//tmpBounds = bounds;

	//SDL_Rect r = { bounds.x + 20, bounds.y + 20, 128, 95 };
	//app->render->DrawRectangle(r, 0, 0, 0, 255, true, false);
	//
	//// Draw buttons
	//btnUse->bounds.x = r.x;
	//btnUse->bounds.y = r.y + 10; // 10 stands for offset
	//btnUse->Draw(app->render, showColliders, 32);

	//btnDelete->bounds.x = r.x;
	//btnDelete->bounds.y = r.y + r.h / 2 + 5; // 5 stands for offset
	//btnDelete->Draw(app->render, showColliders, 32);

	//tmpUsingBounds = r;
}

void Inventory::DragItem(Item& item)
{
	/*int x, y;
	app->input->GetMousePosition(x, y);

	item.bounds.x = x - 16;
	item.bounds.y = y - 16;*/
}

void Inventory::ResetStates()
{
	/*easing->easingsActivated = true;
	state = InventoryState::NONE;

	controls.clear();
	controls.push_back(btnEquipment);
	controls.push_back(btnItems);
	currentControl = btnEquipment;
	lastControl = nullptr;*/
}

void Inventory::GetEquipment(Player* player)
{
	/*equipment[0].item = player->GetHelmet();
	equipment[1].item = player->GetChest();
	equipment[2].item = player->GetBoots();
	for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
	{
		if (equipment[i].item != nullptr)
		{
			equipment[i].filled = true;
			equipment[i].itemsAmount = 1;
		}
		else
		{
			equipment[i].filled = false;
			equipment[i].itemsAmount = 0;
		}
	}*/
}

void Inventory::HandleEquipment(float dt)
{
	/*if (displayEquipmentMenu == false)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			for (int i = 0; i < MAX_EQUIPMENT_SLOTS; i++)
			{
				if (IsMouseInside(equipment[i].bounds) && equipment[i].filled == true)
				{
					displayEquipmentMenu = true;
					tmpEquipMenuBounds = { equipment[i].bounds.x + 20, equipment[i].bounds.y + 20, 128, 50 };
					currentEquipmentId = i;
					break;
				}
			}
		}
	}
	else
	{
		if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !IsMouseInside(tmpEquipMenuBounds)) 
			displayEquipmentMenu = false;

		btnUnEquip->Update(app->input, dt, 1);
	}
}

void Inventory::DisplayMenuEquipment(bool showColliders)
{
	app->render->DrawRectangle(tmpEquipMenuBounds, 0, 0, 0, 255, true, false);

	
	btnUnEquip->bounds.x = tmpEquipMenuBounds.x;
	btnUnEquip->bounds.y = tmpEquipMenuBounds.y + tmpEquipMenuBounds.h / 4;

	btnUnEquip->Draw(app->render, showColliders, 32);*/
}

void Inventory::CompleteOrb(int index)
{
	/*RELEASE(slots[index].item);
	orb = new Orb(atlasTexture);
	slots[index].item = orb;
	slots[index].itemsAmount = 1;*/
}

void Inventory::HandleObjects(InventorySlot objects[])
{
	//if (!grabbed)
	//{
	//	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	//	{
	//		if ((objects[i].itemsAmount > 0) && (IsMouseInside(objects[i].bounds)))
	//		{
	//			// Pop the Use and Delete menu
	//			if (!isTextDisplayed && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	//			{
	//				currentSlotId = i;
	//				objects[i].state = SlotState::SELECTED;
	//				isTextDisplayed = true;
	//				usingItem = true;
	//				break;
	//			}
	//			// Drag Items
	//			if (usingItem == false && !isTextDisplayed && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	//			{
	//				if (objects[i].item != nullptr && objects[i].filled)
	//				{
	//					originSlot = &objects[i];
	//					objects[i].filled = false;
	//					objects[i].item->isDragging = true;
	//					grabbed = true;
	//					objects[i].state = SlotState::NONE;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	// Drop the item to a new Inventory Slot
	//	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	//	{
	//		for (int j = 0; j < MAX_INVENTORY_SLOTS; ++j)
	//		{
	//			if (IsMouseInside(objects[j].bounds) && !objects[j].filled)
	//			{
	//				objects[j].item = originSlot->item;
	//				objects[j].itemsAmount = originSlot->itemsAmount;
	//				objects[j].filled = true;
	//				objects[j].item->isDragging = false;
	//				objects[j].item->bounds = objects[j].bounds;
	//				objects[j].state = SlotState::UNSELECTED;

	//				if (slots[j].id != originSlot->id)
	//				{
	//					objects[originSlot->id].item->isDragging = false;
	//					objects[originSlot->id].filled = false;
	//					objects[originSlot->id].item = nullptr;
	//				}

	//				originSlot = nullptr;

	//				grabbed = false;

	//				break;
	//			}
	//		}
	//	}
	//}

	//// Drag the item. This is done here because otherwise there is one frame that the item's position is not uploaded
	//if (grabbed && originSlot != nullptr)
	//{
	//	DragItem(*objects[originSlot->id].item);
	//}
}

void Inventory::DrawObjects(InventorySlot objects[], Font *font, bool showColliders)
{
	//SDL_Rect r{ 0,0,0,0 };

	//std::string iQuantity;

	//for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	//{
	//	r = { 163, 715, 40, 40 };
	//	app->render->DrawTexture(atlasTexture, objects[i].bounds.x + btnEquipment->bounds.x - SLOT_OFFSET, objects[i].bounds.y, &r, false);
	//	if (showColliders)
	//	{
	//		r = objects[i].bounds;
	//		r.x += btnEquipment->bounds.x - SLOT_OFFSET;

	//		app->render->DrawRectangle(r, 255, 0, 0, 120, true, false);

	//	}
	//	if (objects[i].itemsAmount > 0)
	//	{
	//		SDL_Rect textureRect = { 226,289,32,32 };
	//		iQuantity = std::to_string(objects[i].itemsAmount);

	//		// Draw Item
	//		if (objects[i].item != nullptr && !objects[i].item->isDragging)
	//		{
	//			app->render->DrawTexture(atlasTexture, objects[i].bounds.x + btnEquipment->bounds.x - 156, objects[i].bounds.y + 4, &objects[i].item->atlasSection, false);

	//			app->render->DrawText(font, iQuantity.c_str(), (objects[i].bounds.x + btnEquipment->bounds.x - 156 + objects[i].bounds.w) - 13, (objects[i].bounds.y + objects[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
	//			app->render->DrawText(font, iQuantity.c_str(), (objects[i].bounds.x + btnEquipment->bounds.x - 156 + objects[i].bounds.w) - 15, (objects[i].bounds.y + objects[i].bounds.h) - 25, 25, 2, { 255,255,255 });
	//			if (showColliders) app->render->DrawRectangle(objects[i].item->bounds, 0, 0, 255, 120, true, false);
	//		}
	//	}
	//}

	//if (objects[currentSlotId].state == SlotState::FOCUSED)
	//{
	//	r = objects[currentSlotId].bounds;
	//	r.x += btnEquipment->bounds.x - SLOT_OFFSET;
	//	app->render->DrawRectangle(r, 200, 200, 200, 50, true, false);
	//}
	//else if (equipment[currentArmorSlotId].state == SlotState::FOCUSED)
	//{
	//	r = equipment[currentArmorSlotId].bounds;
	//	r.x += btnEquipment->bounds.x - SLOT_OFFSET + 5;
	//	app->render->DrawRectangle(r, 200, 200, 200, 50, true, false);
	//}

	//// Draw the grabbed object
	//if (originSlot != nullptr && grabbed)
	//{
	//	iQuantity = std::to_string(originSlot->itemsAmount);
	//	app->render->DrawTexture(atlasTexture, originSlot->item->bounds.x + 4, originSlot->item->bounds.y + 4, &originSlot->item->atlasSection, false);
	//	app->render->DrawText(font, iQuantity.c_str(), (originSlot->item->bounds.x + originSlot->item->bounds.w) - 13, (originSlot->item->bounds.y + originSlot->item->bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
	//	app->render->DrawText(font, iQuantity.c_str(), (originSlot->item->bounds.x + originSlot->item->bounds.w) - 15, (originSlot->item->bounds.y + originSlot->item->bounds.h) - 25, 25, 2, { 255,255,255 });
	//	if (showColliders) app->render->DrawRectangle(originSlot->item->bounds, 0, 255, 0, 120, true, false);
	//}

	//if (currentSlotId > -1 || currentArmorSlotId > -1)
	//{
	//	if (objects[currentSlotId].state == SlotState::SELECTED)
	//		DisplayText(objects[currentSlotId].bounds, showColliders);

	//	if (equipment[currentArmorSlotId].state == SlotState::SELECTED)
	//		DisplayText(equipment[currentArmorSlotId].bounds, showColliders);
	//}
}

void Inventory::HandleSlotState()
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (IsMouseInside(slotList[invSlot_].bounds))
		{
			slotList[invSlot_].state = SlotState::FOCUSED;
		}
		else
		{
			slotList[invSlot_].state = SlotState::UNSELECTED;
		}
	}
}

void Inventory::HandleStatsInfo()
{
	//mouse input
	if (IsMouseInside(boundsLeftArrow) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (showStatsId == 2)
		{
			showStatsId--;
		}
		SDL_ShowCursor(SDL_ENABLE);
	}
	if (IsMouseInside(boundsRightArrow) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (showStatsId == 1)
		{
			showStatsId++;
		}
		SDL_ShowCursor(SDL_ENABLE);
	}
	//controller input
	if (CONTROLLERBACKLEFT)
	{
		if (showStatsId == 2)
		{
			showStatsId--;
		}
		SDL_ShowCursor(SDL_DISABLE);
	}
	if (CONTROLLERBACKRIGHT)
	{
		if (showStatsId == 1)
		{
			showStatsId++;
		}
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void Inventory::UseObject(InventorySlot objects[], Player* player)
{
	/*objects[currentSlotId].item->UseItem(player);
	--objects[currentSlotId].itemsAmount;

	if (objects[currentSlotId].itemsAmount > 0)
	{
		objects[currentSlotId].state = SlotState::SELECTED;
		isTextDisplayed = true;
	}
	else
	{
		objects[currentSlotId].state = SlotState::UNSELECTED;
		objects[currentSlotId].filled = false;
		isTextDisplayed = false;
		if (objects[currentSlotId].item->objectType == ObjectType::ARMOR)
		{
			objects[currentSlotId].item = nullptr;
		}
	}*/
}

void Inventory::UseObject(ItemType itemType)
{
	/*int aux;
	int aux2;
	int counter = 0;
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (slots[i].item != nullptr && slots[i].item->itemType == itemType)
		{
			aux = ObjectQuantity(itemType) / ITEM_STACK;
			aux2 = ObjectQuantity(itemType) - (aux * ITEM_STACK);

			if (aux == 0 && aux2 == slots[i].itemsAmount)
			{
				slots[i].itemsAmount--;

				if (slots[i].itemsAmount <= 0)
				{
					slots[i].state = SlotState::UNSELECTED;
					slots[i].filled = false;
					isTextDisplayed = false;
					slots[i].item = nullptr;

					break;
				}
			}
			else if (aux > 0 && slots[i].itemsAmount == aux2)
			{
				slots[i].itemsAmount--;

				if (slots[i].itemsAmount <= 0)
				{
					slots[i].state = SlotState::UNSELECTED;
					slots[i].filled = false;
					isTextDisplayed = false;
					slots[i].item = nullptr;

					break;
				}
			}
			else if (aux > 0 && slots[i].itemsAmount == ITEM_STACK)
			{
				++counter;

				if (aux == counter && aux2 == 0)
				{
					slots[i].itemsAmount--;

					if (slots[i].itemsAmount <= 0)
					{
						slots[i].state = SlotState::UNSELECTED;
						slots[i].filled = false;
						isTextDisplayed = false;
						slots[i].item = nullptr;

						break;
					}
				}
			}
		}
	}*/
}

int Inventory::ObjectQuantity(ItemType itemType)
{
	int counter = 0;
	/*for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (slots[i].item != nullptr && slots[i].item->itemType == itemType)
		{
			++counter;
		}
		else if (armorSlots[i].item != nullptr && armorSlots[i].item->itemType == itemType)
		{
			++counter;
		}
	}*/
	return counter;
}

void Inventory::DrawStatsInfo(bool showColliders)
{
}