#pragma once

#include "Hud.h"
#include "Item.h"

#include "GuiButton.h"

#include <iostream>

#define MAX_INVENTORY_SLOTS 10
#define MAX_EQUIPMENT_SLOTS 3
#define ITEM_STACK 32

class Input;
class Player;
class SDL_Texture;
class Orb;
class Easing;
class SceneGameplay;

enum class PlayerType;

enum class SlotStateShop
{
	NONE = 0,
	UNSELECTED,
	SELECTED,
	FOCUSED,
	USE,
	DELETE
};

enum class InventoryShopState
{
	NONE = 0,
	EQUIPMENT,
	ITEMS,
	STATS,
};

struct InventoryShopSlot
{
	SDL_Rect bounds;
	SDL_Rect itemTextureBounds;
	Item* item;
	int itemsAmount;
	bool isfull;
	int currentSlot;
	int itemId; //if zero empty
	SlotStateShop state;

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

class InventoryShop : public Module
{
public:
	InventoryShop(bool startEnabled);
	virtual ~InventoryShop();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	void Draw();
	bool CleanUp() override;
	bool OnMouseClickEvent();
	void UpdatingButtons(Input* input);

	bool LoadState(pugi::xml_node& node);
	bool SaveState(pugi::xml_node& node);

	void AddItem(Item *it);

	bool IsMouseInside(SDL_Rect r);
	bool IsMouseClickInside(SDL_Rect r, iPoint clickPos);
	void SaveMouseClickPos();

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
	int GetDragonSlayer();
	int GetGrapplingHook();
	int GetBulletPenetration();
	int GetMysticalEnergy();
	int GetArcaneSpirit();

	int GetItemEquipped();

	Player* GetPlayers(ListItem<Player*>* pl_);

private:

	void BuyItem(int curSlot_);

	void DeleteItem(int curSlot_);

	void ItemMenuDraw(int curSlot_);

	void HandleObjects(InventoryShopSlot objects[]);

	void DrawObjects(InventoryShopSlot objects[], Font* font, bool showColliders);

	void HandleSlotState();

	void HandleStatsInfo();

	void UseObject(InventoryShopSlot objects[], Player* player);

	void DrawStatsInfo(bool showColliders);

public:
	List<Player*> players;

	GuiButton* btnEquipment; // Armors
	GuiButton* btnItems;     // Potions etc

	bool isActivated;
	iPoint invPos;
	iPoint mouseClickPos;

	Player* catska;
	Player* gats;

	Easing* easing;

private:
	SDL_Texture* itemTexture;
	SDL_Texture* invShopTexture;
	SDL_Texture* coinTexture;

	SDL_Texture* slotText;
	SDL_Texture* gatsText;
	SDL_Texture* catskaText;
	SDL_Texture* itemMenuTexture;

	int easingPos;

	SDL_Rect gatsRect;
	SDL_Rect catskaRect;

	SDL_Rect slotRect;
	SDL_Rect slotRectFocus;
	SDL_Rect coinRect;

	SDL_Rect itemMenuBounds;
	SDL_Rect itemMenuPos;
	SDL_Rect buyButton;
	SDL_Rect buyButtonFocus;
	SDL_Rect buyButtonPos;

	SDL_Rect leftArrowRect;
	SDL_Rect leftArrowRectFocus;
	SDL_Rect rightArrowRect;
	SDL_Rect rightArrowRectFocus;

	SDL_Rect boundsLeftArrow;
	SDL_Rect boundsRightArrow;

	GuiButton* leftArrow;
	GuiButton* rightArrow;

	int showStatsId;

	SceneGameplay* scene;

	InventoryShopState state;

	// Armor and weapons equiped slots
	/*InventorySlot equipment[MAX_EQUIPMENT_SLOTS];
	bool inEquipment;*/

	// There will be 32 slots for items
	//InventorySlot slots[MAX_INVENTORY_SLOTS];
	int currentSlotId;
	InventoryShopSlot* originSlot;

	// There will be 32 slots for Armor
	/*InventorySlot armorSlots[MAX_INVENTORY_SLOTS];
	int currentArmorSlotId;
	InventorySlot* originArmorSlot;*/

	Player* currentPlayer;

	Orb* orb;

	// Display Text buttons to select with player uses the item
	/*GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;*/

	//List<InventorySlot*> slotList;
	InventoryShopSlot slotList[MAX_INVENTORY_SLOTS];
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

	Easing* easing2;
	int counter;

	//iPoint invPos;
	iPoint invPosText;

	bool grabbed;
	float toGrabCount;
	bool isTextDisplayed;
	bool usingItem;
	bool displayEquipmentMenu;
	int currentEquipmentId;

	int coin;

	bool buyItem_;

	//Item Menu
	SString itName;
	bool isItemMenu_Active;
	int currentSlot_;
};