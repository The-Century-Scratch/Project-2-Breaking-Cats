
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "ModuleCollisions.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include <cmath>
#include "SDL_image/include/SDL_image.h"

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    return true;
}

// L12: Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayer*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayer* layer = item->data;

        if (/*layer->properties.GetProperty("Navigation")->value != NULL &&*/ !layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

void Map::Draw()
{
    if (mapLoaded == false)
        return;

    // Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        // use GetProperty method to ask each layer if your �Draw� property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    // Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    if (mapLayerItem->data->id == 3)
                    {
                        continue;
                    }

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);

                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
}

void Map::DrawAfterPlayer()
{
    if (mapLoaded == false)
        return;

    // Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        // use GetProperty method to ask each layer if your �Draw� property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    // Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    if (mapLayerItem->data->id == 3)
                    {
                        app->render->DrawTexture(tileset->texture,
                            pos.x,
                            pos.y,
                            &r);
                        continue;
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }
}

// Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

// Add method WorldToMap to obtain  map coordinates from screen coordinates
iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // clean up all layer data
    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    mapData.maplayers.Clear();

    ListItem<ObjectGroup*>* ObjectGroupItem;
    ObjectGroupItem = mapData.mapObjectGroups.start;

    while (ObjectGroupItem != NULL)
    {
        ListItem<Object*>* ObjectItem;
        ObjectItem = ObjectGroupItem->data->objects.start;

        while (ObjectItem != NULL)
        {
            RELEASE(ObjectItem->data);
            ObjectItem = ObjectItem->next;
        }
        ObjectGroupItem->data->objects.Clear();

        RELEASE(ObjectGroupItem->data);
        ObjectGroupItem = ObjectGroupItem->next;
    }
    mapData.mapObjectGroups.Clear();

    //REMOVE ALL COLLIDERS
    app->moduleCollisions->CleanUp(true);



    return true;
}

// Load new map
bool Map::Load(const char* scene)
{

    LOG("Loading Map ");
    bool ret = true;

    pugi::xml_node configNode = app->LoadConfigFileToVar();
    pugi::xml_node config = configNode.child(scene).child(name.GetString());

    //mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    for (pugi::xml_node nodeMapPath = config.child("mapfile");
        nodeMapPath; nodeMapPath = nodeMapPath.next_sibling("mapfile"))
    {
        mapFileName.Add(nodeMapPath.attribute("path").as_string());
    }
    for (size_t i = 0; i < mapFileName.Count(); i++)
    {
        LOG("String of Path saved: %s", mapFileName[i].GetString());
    }

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName[app->sceneManager->currentScene].GetString());


    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    if (ret == true)
    {
        ret = LoadAllObjectGroups(mapFileXML.child("map"));
    }
    
    // Create colliders
    CreateColliders(mapFileXML);

    if(ret == true)
    {
        // LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", mapFileName[app->sceneManager->currentScene].GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

void Map::ClearMaps()
{
    mapFileName.Clear();
}

// Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    // Read the prientation of the map
    mapData.type = MAPTYPE_UNKNOWN;
    if (strcmp(map.attribute("orientation").as_string(), "isometric") == 0)
    {
        mapData.type = MAPTYPE_ISOMETRIC;
    }
    if (strcmp(map.attribute("orientation").as_string(), "orthogonal") == 0)
    {
        mapData.type = MAPTYPE_ORTHOGONAL;
    }

    return ret;
}

// Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// Lbool ret = true;

    // Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;
    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    // Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadObject(pugi::xml_node& node, Object* object)
{
    bool ret = true;
    int arrLenght = 0;
    //Load the attributes
    object->id = node.attribute("id").as_int();
    object->x = node.attribute("x").as_int();
    object->y = node.attribute("y").as_int();

    // Call Load Propoerties
    SString polygonString;
    polygonString = node.child("polygon").attribute("points").as_string();


    //Reserve the memory for the data 
    for (int a = 0; a < polygonString.Length(); a++, arrLenght++)
    {
        if ((polygonString.GetTerm(a) != ' ') && (polygonString.GetTerm(a) != ','))
        {
            arrLenght--;
        }
    }

    object->chainPoints = new int[arrLenght];
    memset(object->chainPoints, 0, arrLenght);

    
    //char* temp = strtok(polygonString.GetCharString(), " ");
    char* temp;
    int arr[100];
    int count = 0;
    int j = 0;
    bool negative = false;
    /*SString clearString;
    while (temp != NULL)
    {
        clearString += temp;
    }*/
    LOG("number %s", polygonString.GetString());
    for (uint i = 0; i < polygonString.Length(); i++,j++)
    {
        //LOG("number %s", polygonString.GetTerm(i));
        if ((polygonString.GetTerm(i) != ' ') && (polygonString.GetTerm(i) != ','))
        {
            if(polygonString.GetTerm(i) == '-')
            {
                negative = true;
            }
            else
            {
                
                arr[count] = ((int)polygonString.GetTerm(i)) - 48;
                LOG("%i", arr[count]);
                if (negative == true)
                {
                    arr[count] *= -1;
                    negative = false;
                }
                count++;
            }
            
            j--;
        }
        else
        {
            count--;
            int aux = 0;
            for (int a = 0; count >= 0; a++, count--)
            {
                if (aux < 0)
                {
                    aux -= arr[a] * (pow(10, count));
                }
                else
                {
                    aux += arr[a] * (pow(10, count));
                }
                
            }
            object->chainPoints[j] = aux;
            LOG("AUX NUMBER %i", object->chainPoints[j]);
            count = 0;
        }
    }
    count--;
    int aux = 0;
    for (int a = 0; count >= 0; a++, count--)
    {
        aux += arr[a] * (pow(10, count));
    }
    object->chainPoints[j] = aux;
    object->size = arrLenght + 1;
    LOG("AUX NUMBER %i", object->chainPoints[j]);

    polygonString.Clear();
    
    return ret;
}

bool Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectGroup)
{
    bool ret = true;
    //Load the attributes
    objectGroup->id = node.attribute("id").as_int();
    objectGroup->name = node.attribute("name").as_string();


    for (pugi::xml_node objectNode = node.child("object"); objectNode && ret; objectNode = objectNode.next_sibling("object"))
    {
        //Load the object
        Object* mapObject = new Object();
        ret = LoadObject(objectNode, mapObject);

        //add the object to the map
        objectGroup->objects.Add(mapObject);
    }

    return ret;
}

bool Map::LoadAllObjectGroups(pugi::xml_node mapNode)
{
    bool ret = true;

    for (pugi::xml_node objectGroupNode = mapNode.child("objectgroup"); objectGroupNode && ret; objectGroupNode = objectGroupNode.next_sibling("objectgroup"))
    {
        //Load the objectGroup
        ObjectGroup* mapObjectGroup = new ObjectGroup();
        ret = LoadObjectGroup(objectGroupNode, mapObjectGroup);

        //add the objectGroup to the map
        mapData.mapObjectGroups.Add(mapObjectGroup);
    }

    return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

bool Map::CreateColliders(pugi::xml_node mapFile) // it creates the collisions lol omg xd
{
    bool ret = true;
    int id;
    int newscene = 1;
    //CREATE TILE COLLIDERS
    pugi::xml_node objectGroupNode = mapFile.child("map").child("objectgroup");

    SDL_Rect rect;

    while (objectGroupNode != NULL)
    {
        pugi::xml_node objectNode = objectGroupNode.child("object");

        if (objectGroupNode.attribute("id").as_int() == 4  /*!strcmp(objectGroupNode.attribute("name").as_string(), "Wall")*/)
        {
            while (objectNode != NULL)
            {
                rect = {    objectNode.attribute("x").as_int(),
                            objectNode.attribute("y").as_int(),
                            objectNode.attribute("width").as_int(),
                            objectNode.attribute("height").as_int() };

                Collider* c1 = app->moduleCollisions->AddCollider(rect,Collider::Type::WALL, nullptr, true);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (objectGroupNode.attribute("id").as_int() == 5   /*!strcmp(objectGroupNode.attribute("name").as_string(), "ChangeScene")*/)
        {
            while (objectNode != NULL)
            {
                rect = {    objectNode.attribute("x").as_int(),
                            objectNode.attribute("y").as_int(),
                            objectNode.attribute("width").as_int(),
                            objectNode.attribute("height").as_int() };

                id = objectNode.attribute("id").as_int();
                if (app->sceneManager->currentScene == 0)
                {
                    switch (id)
                    {
                    case 54:
                        newscene = 5;
                        break;
                    case 55:
                        newscene = 7;
                        break;
                    case 56:
                        newscene = 3;
                        break;
                    case 57:
                        newscene = 2;
                        break;
                    case 58:
                        newscene = 1;
                        break;
                    default:
                        break;
                    }
                }
                if (app->sceneManager->currentScene == 4)
                {
                    switch (id)
                    {
                    case 18:
                        newscene = 6;
                        break;
                    case 19:
                        newscene = 5;
                        break;
                    default:
                        break;
                    }
                }
                if (app->sceneManager->currentScene == 5)
                {
                    switch (id)
                    {
                    case 21:
                        newscene = 4;
                        break;
                    case 22:
                        newscene = 0;
                        break;
                    default:
                        break;
                    }
                }
                if (app->sceneManager->currentScene == 7)
                {
                    switch (id)
                    {
                    case 59:
                        newscene = 0;
                        break;
                    case 60:
                        newscene = 8;// future lab that is not implemented
                    default:
                        break;
                    }
                }

                Collider* c1 = app->moduleCollisions->AddCollider(rect, Collider::Type::CHANGESCENE, nullptr, true, newscene);

                objectNode = objectNode.next_sibling("object");
            }
        }
        objectGroupNode = objectGroupNode.next_sibling("objectgroup");
    }

    
    
    return ret;
}


// Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


