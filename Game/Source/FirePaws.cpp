#include "App.h"
#include "Textures.h"
#include "Render.h"
//#include "ModuleCollisions.h"

#include "Player.h"

#include "FirePaws.h"

FirePaws::FirePaws(iPoint pos, SDL_Texture* itemText_) : Item(pos, itemText_)
{

	itemTextSection = { 0, 0, 32, 32};
	itemType = ItemType::FIRE_PAWS;
}

FirePaws::~FirePaws()
{
}

bool FirePaws::Start()
{
	//eCollider = app->moduleCollisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::ITEM, (Entity*)this);
	//currentAnim = &idleanim;
	itemid = 1;
	itemType = ItemType::FIRE_PAWS;

	return true;
}

bool FirePaws::Update()
{
	return true;
}

bool FirePaws::PostUpdate()
{
	Draw();
	return true;
}

void FirePaws::Draw()
{
	//app->render->DrawTexture(texture, position.x, position.y, &itemTextSection);
}

bool FirePaws::CleanUp()
{
	app->tex->Unload(texture);
	return true;
}

bool FirePaws::SaveState(pugi::xml_node& n)
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

void FirePaws::UseItem(Player* player)
{
	//player->GetHealed(healAmount);
}