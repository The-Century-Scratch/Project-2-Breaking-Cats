#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "EASTL/vector.h"

#include "PugiXml\src\pugixml.hpp"

// Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	// Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	// Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	List<Property*> list;
};

// Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	// Store custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// Short function to get the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct Object
{
	int id = 0;
	int x = 0, y = 0;
	int* chainPoints = 0;
	int size = 0;

	inline int Get(int x) const
	{
		return chainPoints[x];
	}
	Object() : chainPoints(NULL)
	{}

	~Object()
	{
		RELEASE(chainPoints);
	}
};
struct ObjectGroup
{
	SString	name;
	int id;

	List<Object*> objects;
};

// Create a struct needed to hold the information to Map node
struct MapData
{
	int width = 0;
	int	height = 0;
	int	tileWidth = 0;
	int	tileHeight = 0;
	List<TileSet*> tilesets;
	MapTypes type;

	//: Add a list/array of layers to the map
	List<MapLayer*> maplayers;

	List<ObjectGroup*> mapObjectGroups;
	
};

struct MapProperties
{
	bool fixedCameraX = 0;
	bool fixedCameraY = 0;

	iPoint initialCamera;
};


class Map : public Module
{
public:

    Map(bool startEnabled);

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

	// Called each loop iteration
	void DrawAfterPlayer();
	void DrawAfterItem();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* scene);

	// Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y);

	// L12: Create walkability map for pathfinding
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	bool CreateColliders(pugi::xml_node mapFile);

	void ClearMaps();

private:

	bool LoadMap(pugi::xml_node mapFile);

	// Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	// L05
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	bool LoadObject(pugi::xml_node& node, Object* object);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectGroup);
	bool LoadAllObjectGroups(pugi::xml_node mapNode);

	TileSet* GetTilesetFromTileId(int gid) const;

	// Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// Load Map Custom properties 
	bool LoadMapProperties(pugi::xml_node& node);
	

public: 

	// Declare a variable data of the struct MapData
	MapData mapData;
	// Custom map properties loaded from tiled
	MapProperties mapProperties;

	eastl::vector<pugi::xml_node> propertyNodes;

	//List<Collider*> mapColliders;

private:

	List<SString> mapFileName;
	SString mapFolder;
    bool mapLoaded = 0;
};

#endif // __MAP_H__