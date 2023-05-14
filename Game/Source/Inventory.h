#pragma once

#include "Hud.h"
#include "Item.h"

#include "GuiButton.h"

#include <iostream>

#define MAX_INVENTORY_SLOTS 16
#define MAX_EQUIPMENT_SLOTS 3
#define ITEM_STACK 32

class Input;
class Player;
class SDL_Texture;
class Orb;
class Easing;
class SceneGameplay;

enum class PlayerType;

enum class SlotState
{
	NONE = 0,
	UNSELECTED,
	SELECTED,
	FOCUSED,
	USE,
	DELETE
};

enum class InventoryState
{
	NONE = 0,
	EQUIPMENT,
	ITEMS,
	STATS,
};

struct InventorySlot
{
	SDL_Rect bounds;
	SDL_Rect itemTextureBounds;
	Item* item;
	int itemsAmount;
	bool isfull;
	int currentSlot;
	int itemId; //if zero empty
	SlotState state;

	/*bool operator==(InventorySlot& i) const
	{
		return (i.bounds.x == bounds.x && i.bounds.y == bounds.y && i.bounds.w == bounds.w && i.bounds.h == bounds.h &&
			i.item->objectType == item->objectType && i.itemsAmount == itemsAmount && i.filled == filled);
	}
	bool operator!=(InventorySlot& i) const
	{
		return (i.bounds.x != bounds.x || i.bounds.y != bounds.y || i.bounds.w != bounds.w || i.bounds.h != bounds.h ||
			i.item->objectType != item->objectType || i.itemsAmount != itemsAmount || i.filled != filled);
	}*/
};

class Inventory : public Module
{
public:
	Inventory(bool startEnabled);
	virtual ~Inventory();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	void Draw();
	bool CleanUp() override;
	bool OnGuiMouseClickEvent(GuiControl* control);
	void UpdatingButtons(Input* input);

	/*void LoadState(pugi::xml_node& node);
	void SaveState(pugi::xml_node& node);*/

	void AddItem(Item *it);

	bool IsMouseInside(SDL_Rect r);

	void DisplayText(SDL_Rect bounds, bool showColliders);

	void DragItem(Item& item);

	void GetEquipment(Player* player);

	void HandleEquipment(float dt);

	void DisplayMenuEquipment(bool showColliders);

	void CompleteOrb(int index);

	int ObjectQuantity(ItemType itemType);

	void UseObject(ItemType itemType);

	void ResetStates();

	int GetFirePaw();

	int GetItemEquipped();

private:

	void HandleObjects(InventorySlot objects[]);

	void DrawObjects(InventorySlot objects[], Font* font, bool showColliders);

	void HandleSlotState();

	void UseObject(InventorySlot objects[], Player* player);

	void HandleStatsInfo();

	void DrawStatsInfo(bool showColliders);

public:
	//eastl::list<Player*> players;

	GuiButton* btnEquipment; // Armors
	GuiButton* btnItems;     // Potions etc

private:
	SDL_Texture* itemTexture;
	SDL_Texture* invTex;
	SDL_Texture* slotText;
	SDL_Texture* buttonTex;

	SDL_Rect slotRect;
	SDL_Rect slotRectFocus;

	SceneGameplay* scene;

	InventoryState state;

	// Armor and weapons equiped slots
	InventorySlot equipment[MAX_EQUIPMENT_SLOTS];
	bool inEquipment;

	// There will be 32 slots for items
	//InventorySlot slots[MAX_INVENTORY_SLOTS];
	int currentSlotId;
	InventorySlot* originSlot;

	// There will be 32 slots for Armor
	InventorySlot armorSlots[MAX_INVENTORY_SLOTS];
	int currentArmorSlotId;
	InventorySlot* originArmorSlot;

	Player* currentPlayer;

	Orb* orb;

	// Display Text buttons to select with player uses the item
	/*GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;*/

	//List<InventorySlot*> slotList;
	InventorySlot slotList[MAX_INVENTORY_SLOTS];
	GuiButton* btnUse;       // Use item
	GuiButton* btnDelete;	 // Delete an item
	//GuiButton* btnEquip;	// Equips an armor
	GuiButton* btnUnEquip;	// Unequip an armor

	// Use this buttons for going through characters
	GuiButton* btnNext;
	GuiButton* btnPrev;
	GuiButton* btnNextStats;
	GuiButton* btnPrevStats;

	SDL_Rect tmpBounds;
	SDL_Rect tmpUsingBounds;
	SDL_Rect tmpCharBounds;
	SDL_Rect tmpEquipMenuBounds;

	Easing* easing;
	Easing* easing2;
	int counter;

	iPoint invPos;
	iPoint invPosText;

	bool grabbed;
	float toGrabCount;
	bool isTextDisplayed;
	bool usingItem;
	bool displayEquipmentMenu;
	int currentEquipmentId;
};