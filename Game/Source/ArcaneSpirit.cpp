#include "App.h"
#include "Textures.h"
#include "Render.h"
//#include "ModuleCollisions.h"

#include "Player.h"

#include "ArcaneSpirit.h"

ArcaneSpirit::ArcaneSpirit(iPoint pos, SDL_Texture* itemText_) : Item(pos, itemText_)
{

	itemTextSection = { 0, 0, 16, 16 };
	itemType = ItemType::ARCANE_SPIRIT;
}

ArcaneSpirit::~ArcaneSpirit()
{
}

bool ArcaneSpirit::Start()
{
	//eCollider = app->moduleCollisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::ITEM, (Entity*)this);
	//currentAnim = &idleanim;
	itemid = 6;

	return true;
}

bool ArcaneSpirit::Update()
{
	return true;
}

bool ArcaneSpirit::PostUpdate()
{
	Draw();
	return true;
}

void ArcaneSpirit::Draw()
{
	//app->render->DrawTexture(texture, position.x, position.y, &itemTextSection);
}

bool ArcaneSpirit::CleanUp()
{
	app->tex->Unload(texture);
	return true;
}

bool ArcaneSpirit::SaveState(pugi::xml_node& n)
{
	/*pugi::xml_node node = n.append_child("position");
	node.append_attribute("x").set_value(bounds.x);
	node.append_attribute("y").set_value(bounds.y);

	node = n.append_child("object_type");
	node.append_attribute("value").set_value((int)objectType);

	node = n.append_child("item_type");
	node.append_attribute("value").set_value((int)itemType);

	node = n.append_child("map_name");
	node.append_attribute("value").set_value(mapName.c_str());*/

	return true;
}

void ArcaneSpirit::UseItem(Player* player)
{
	//player->GetHealed(healAmount);
}