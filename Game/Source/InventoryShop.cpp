#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Window.h"
#include "Debug.h"
#include "Inventory.h"
#include "InventoryShop.h"
#include "SceneGameplay.h"
#include "GuiManager.h"
#include "StatsManager.h"
#include "SceneManager.h"
#include "QuestManager.h"

#include "Item.h"

#include "Easings.h"

#define SLOT_OFFSET 160

InventoryShop::InventoryShop(bool startEnabled) : Module(startEnabled)
{
	name.Create("InventoryShop");
}

InventoryShop::~InventoryShop()
{
}

bool InventoryShop::Start()
{
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	itemTexture =	app->tex->Load(config.child("itemTexture").attribute("texturepath").as_string());
	invShopTexture =		app->tex->Load(config.child("invShopTexture").attribute("texturepath").as_string());
	coinTexture = app->tex->Load(config.child("coinTexture").attribute("texturepath").as_string());

	//slotText =		app->tex->Load(config.child("slotTexture").attribute("texturepath").as_string());
	gatsText =		app->tex->Load(config.child("gatsTexture").attribute("texturepath").as_string());
	catskaText =	app->tex->Load(config.child("catskaTexture").attribute("texturepath").as_string());
	itemMenuTexture =	app->tex->Load(config.child("itemMenuTexture").attribute("texturepath").as_string());


	isActivated = false;
	isItemMenu_Active = false;
	buyItem_ = false;
	coins = 0;

	showStatsId = 1;

	slotRect =				{ 0,  240, 36, 36 };
	slotRectFocus =			{ 36, 240, 36, 36 };
	coinRect =				{ 5,  5,   22, 22 };

	itemMenuBounds =		{ 0,   0,   104, 115 };
	itemMenuPos =			{ 0,   0,   0,   0   };
	buyButton =				{ 73,  240, 100, 25  };
	buyButtonFocus =		{ 173, 240, 100, 25  };
	buyButtonPos =		{ 136, 203, 100, 25  };

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
		slotList[invSlot_].state = SlotStateShop::UNSELECTED;
	}

	easing = new Easing(false, 0, -40, 40, 100);

	return true;
}

bool InventoryShop::Update(float dt)
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
			slotList[invSlot_].bounds.x = invPos.x + invSpacing.x + 58;
			slotList[invSlot_].bounds.y = invPos.y + invSpacing.y + 84;
			invSpacing.x += 56;
			if (invSlot_ == 4) invSpacing = { 0, invSpacing.y += 66 };
		}

		boundsLeftArrow.x = invPos.x + 20;
		boundsLeftArrow.y = invPos.y + 93;
		boundsRightArrow.x = invPos.x + 154;
		boundsRightArrow.y = invPos.y + 93;

		buyButtonPos.x = invPos.x + 136;
		buyButtonPos.y = invPos.y + 203;

		HandleStatsInfo();

		HandleSlotState();

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
			SaveMouseClickPos();
			OnMouseClickEvent();
		}

		if (buyItem_)
		{
			BuyItem(currentSlot_);
		}

		if (app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN)
		{
			++coins;
		}

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN)
		{
			--coins;
		}

	}

	return true;
}

bool InventoryShop:: PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_P) == KeyState::KEY_DOWN && app->hud->hudstate == hudSTATE::CLOSED && !app->questManager->printQuestMenu)
	{
		isActivated = !isActivated;
		app->sceneManager->Pause = isActivated;
		app->audio->PlayFx(app->hud->swapscenesfx);
		SDL_ShowCursor(SDL_ENABLE);
		easing->easingsActivated = true;
		easing->currentIteration = 0;
		app->inventory->isActivated = false;
	}

	if (isActivated)
	{
		Draw();
	}

	return true;
}

void InventoryShop::BuyItem(int curSlot_)
{

	if (IsMouseInside(buyButtonPos) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (coins >= 1)
		{
			app->inventory->AddItem(slotList[curSlot_].item);
			DeleteItem(curSlot_);
			coins--;
		}
		buyItem_ = false;
	}
}

void InventoryShop::DeleteItem(int curSlot_)
{
	slotList[curSlot_].itemId = 0;
	delete(slotList[curSlot_].item);
	slotList[curSlot_].item = nullptr;
}

void InventoryShop::ItemMenuDraw(int curSlot_)
{
	int scale = app->win->GetScale();
	SDL_Color white = { 255, 255, 255, 255 };
	app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5, &itemMenuBounds);

	if (IsMouseInside(buyButtonPos))
	{
		app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 88, &buyButtonFocus);
	}
	else
	{
		app->render->DrawTexture(itemMenuTexture, slotList[curSlot_].bounds.x - itemMenuBounds.w, slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 88, &buyButton);
	}

	if (slotList[curSlot_].item->equiped)
	{
		app->render->DrawText("UNEQUIP", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 5) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 91) * scale, 94 * scale, 19 * scale, white);
	}
	else
	{
		app->render->DrawText(" EQUIP ", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 5) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 91) * scale, 94 * scale, 19 * scale, white);
	}

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
		app->render->DrawText("new  information", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("very  soon...", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	case 6:
		app->render->DrawText("Arcane  Spirit", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 10) * scale, 84 * scale, 14 * scale, white);
		app->render->DrawText("new  information", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 38) * scale, 84 * scale, 10 * scale, white);
		app->render->DrawText("very  soon...", app->render->camera.x + (slotList[curSlot_].bounds.x - itemMenuBounds.w + 10) * scale, app->render->camera.y + (slotList[curSlot_].bounds.y - itemMenuBounds.h * 0.5 + 55) * scale, 84 * scale, 10 * scale, white);
		break;
	default:
		break;
	}
}

int InventoryShop::GetItemEquipped()
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

Player* InventoryShop::GetPlayers(ListItem<Player*>* pl_)
{
	return currentPlayer;
}

int InventoryShop::GetFirePaw() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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

int InventoryShop::GetDragonSlayer() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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


int InventoryShop::GetGrapplingHook() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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

int InventoryShop::GetBulletPenetration() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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

int InventoryShop::GetMysticalEnergy() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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

int InventoryShop::GetArcaneSpirit() // TODO: Ideally, this would check the equipment slot and return the item, that way it would be more versatile
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

void InventoryShop::Draw()
{
	int scale = app->win->GetScale();
	SDL_Color white = {255, 255, 255, 255};
	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Rect invRect = { 0, 0, 372, 240 };
	app->render->DrawTexture(invShopTexture, invPos.x, invPos.y, &invRect);
	//app->render->DrawText("INVENTORY",invPosText.x + 222 * scale, invPosText.y + 27 * scale, 92 * scale, 24 * scale, white);
	//app->render->DrawText("STATS", invPosText.x + 77 * scale, invPosText.y + 127 * scale, 38 * scale, 11 * scale, white);
	//app->render->DrawText(std::to_string(app->statsManager->gats_hp).c_str(), invPosText.x + 100 * scale, invPosText.y + 156 * scale, 30 * scale, 18 * scale, white);


	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		app->render->DrawTexture(invShopTexture, slotList[invSlot_].bounds.x, slotList[invSlot_].bounds.y, &slotRect);

		switch (slotList[invSlot_].state)
		{
		case SlotStateShop::UNSELECTED:
			break;
		case SlotStateShop::FOCUSED:
			app->render->DrawTexture(invShopTexture, slotList[invSlot_].bounds.x, slotList[invSlot_].bounds.y, &slotRectFocus);
			break;
		case SlotStateShop::SELECTED:
			break;
		default:
			break;
		}

		if (slotList[invSlot_].itemId != 0)
		{
			app->render->DrawTexture(itemTexture, slotList[invSlot_].bounds.x+2, slotList[invSlot_].bounds.y+2, &slotList[invSlot_].itemTextureBounds);
		}

		if (isItemMenu_Active)
		{
			ItemMenuDraw(currentSlot_);
		}

		if (IsMouseInside(buyButtonPos))
		{
			app->render->DrawTexture(invShopTexture, invPos.x + 136, invPos.y + 203, &buyButtonFocus);
		}
		else
		{
			app->render->DrawTexture(invShopTexture, invPos.x + 136, invPos.y + 203, &buyButton);
		}

		app->render->DrawText(std::to_string(coins).c_str(), invPosText.x + 286 * scale, invPosText.y + 37 * scale, 21 * scale, 18 * scale, white);

		app->render->DrawText("       BUY       ", invPosText.x + 143 * scale, invPosText.y + 206 * scale, 86 * scale, 19 * scale, black);

	}
}

bool InventoryShop::CleanUp()
{
	//RELEASE(easing);
	app->tex->Unload(invShopTexture);
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

bool InventoryShop::OnMouseClickEvent()
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (IsMouseClickInside(slotList[invSlot_].bounds, mouseClickPos))
		{
			if (slotList[invSlot_].itemId != 0)
				buyItem_ = true;

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

void InventoryShop::UpdatingButtons(Input* input)
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

bool InventoryShop::LoadState(pugi::xml_node& node)
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

bool InventoryShop::SaveState(pugi::xml_node& node)
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

void InventoryShop::AddItem(Item *it)
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

bool InventoryShop::IsMouseInside(SDL_Rect r)
 {
	SDL_Rect auxR = {r.x-invPos.x+25, r.y-invPos.y, r.w, r.h};

	int x, y;
	app->input->GetMousePosition(x, y); 

	return (x > auxR.x) && (x < auxR.x + auxR.w) && (y > auxR.y) && (y < auxR.y + auxR.h);
}

bool InventoryShop::IsMouseClickInside(SDL_Rect r, iPoint clickPos)
{
	SDL_Rect auxR2 = { r.x - invPos.x + 25, r.y - invPos.y, r.w, r.h };

	return (clickPos.x > auxR2.x) && (clickPos.x < auxR2.x + auxR2.w) && (clickPos.y > auxR2.y) && (clickPos.y < auxR2.y + auxR2.h);
}

void InventoryShop::SaveMouseClickPos()
{
	app->input->GetMousePosition(mouseClickPos.x, mouseClickPos.y);
}

void InventoryShop::DisplayText(SDL_Rect bounds, bool showColliders)
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

void InventoryShop::DragItem(Item& item)
{
	/*int x, y;
	app->input->GetMousePosition(x, y);

	item.bounds.x = x - 16;
	item.bounds.y = y - 16;*/
}

void InventoryShop::ResetStates()
{
	/*easing->easingsActivated = true;
	state = InventoryState::NONE;

	controls.clear();
	controls.push_back(btnEquipment);
	controls.push_back(btnItems);
	currentControl = btnEquipment;
	lastControl = nullptr;*/
}

void InventoryShop::GetEquipment(Player* player)
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

void InventoryShop::HandleEquipment(float dt)
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

void InventoryShop::CompleteOrb(int index)
{
	/*RELEASE(slots[index].item);
	orb = new Orb(atlasTexture);
	slots[index].item = orb;
	slots[index].itemsAmount = 1;*/
}

void InventoryShop::HandleObjects(InventoryShopSlot objects[])
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

void InventoryShop::DrawObjects(InventoryShopSlot objects[], Font *font, bool showColliders)
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

void InventoryShop::HandleSlotState()
{
	for (size_t invSlot_ = 0; invSlot_ < MAX_INVENTORY_SLOTS; invSlot_++)
	{
		if (IsMouseInside(slotList[invSlot_].bounds))
		{
			slotList[invSlot_].state = SlotStateShop::FOCUSED;
		}
		else
		{
			slotList[invSlot_].state = SlotStateShop::UNSELECTED;
		}
	}
}

void InventoryShop::HandleStatsInfo()
{
	if (IsMouseInside(boundsLeftArrow) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (showStatsId == 2)
		{
			showStatsId--;
		}
	}
	if (IsMouseInside(boundsRightArrow) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
	{
		if (showStatsId == 1)
		{
			showStatsId++;
		}
	}
}

void InventoryShop::UseObject(InventoryShopSlot objects[], Player* player)
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

void InventoryShop::UseObject(ItemType itemType)
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

int InventoryShop::ObjectQuantity(ItemType itemType)
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

void InventoryShop::DrawStatsInfo(bool showColliders)
{
}