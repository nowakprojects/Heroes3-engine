#include "pch.h"
#include "World.h"
#include "Tiles.h"

World & world = World::Get();

sf::Vector2i World::guardingCreaturePosition(sf::Vector2i pos) const
{
	const sf::Vector2i originalPos = pos;
	// Give monster at position priority.
	if (!world.isInTheMap(pos))
		return sf::Vector2i(-1, -1);
	const Maps::Tile &posTile = world.GetTile(pos);
	if (posTile.visitable)
	{
		for (auto obj : posTile.objects)
		{
			if (obj->blockVisit)
			{
				if (obj->type == Obj::MONSTER) // Monster
					return pos;
				else
					return sf::Vector2i(-1, -1); //blockvis objects are not guarded by neighbouring creatures
			}
		}
	}

	// See if there are any monsters adjacent.
	bool water = posTile.isWater();

	pos -= sf::Vector2i(1, 1); // Start with top left.
	for (int dx = 0; dx < 3; dx++)
	{
		for (int dy = 0; dy < 3; dy++)
		{
			if (world.isInTheMap(pos))
			{
				const auto & tile = world.GetTile(pos);
				if (tile.visitable && (tile.isWater() == water))
				{
					for (auto obj : tile.objects)
					{
						if (obj->type == Obj::MONSTER ) // Monster being able to attack investigated tile
						{
							return pos;
						}
					}
				}
			}

			pos.y++;
		}
		pos.y -= 3;
		pos.x++;
	}

	return sf::Vector2i(-1, -1);
}
bool World::isInTheMap(const sf::Vector2i& pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
	{
		return false;
	}
	else
	{
		return true;
	}
}

World & World::Get()
{
	static World insideWorld;

	return insideWorld;
}

Maps::Tile & World::GetTile(int x, int y)
{
	return this->vec_tiles[this->width * y + x];
}

Maps::Tile & World::GetTile(int index)
{
	return this->vec_tiles[index];
}

Maps::Tile & World::GetTile(sf::Vector2i pos)
{
	return this->vec_tiles[this->width * pos.y + pos.x];
}

MP2::ObjectInstance* World::GetObject(int id)
{
	for (auto obj : this->vec_objects)
		if (obj->id == id) return obj;
	return nullptr;
}

MP2::ObjectInstance* World::GetObjectByPos(sf::Vector2i pos)
{
	
	return this->GetTile(pos).GetObjectPtr();
}

Kingdom & World::GetKingdom(int color)
{
	switch (color)
	{
	case Color::RED:       return *vec_kingdoms[0];
	case Color::BLUE:      return *vec_kingdoms[1];
	case Color::GREEN:     return *vec_kingdoms[2];
	case Color::YELLOW:    return *vec_kingdoms[3];
	case Color::ORANGE:    return *vec_kingdoms[4];
	case Color::PURPLE:    return *vec_kingdoms[5];
	default: break;
	}
	//temp code
	return *vec_kingdoms[0];
}

HeroInstance * World::getRandomHero()
{
	auto h = allHeroes[std::rand() % allHeroes.size()];
	if (h->ownerColor == Color::UNUSED)
		return h;
	else
		return this->getRandomHero(); // nie dobre rozwiązanie
}

HeroInstance * World::getHeroByName(HeroName name)
{
	for (auto& hero : this->allHeroes)
		if (hero->name == name) return hero;
	return nullptr;
}


int World::w() const
{
	return this->width;
}

int World::h() const
{
	return this->height;
}

void World::initAllHeroes()
{
	HeroInstance* h;
	h = new HeroInstance();
	h->name = HeroName::EDRIC;
	h->instanceName = "Edric";
	h->typeName = "";
	h->subType = HeroClass::KNIGHT;
	h->subTypeName = "Knight";
	h->initObj();
	this->allHeroes.push_back(h);

	h = new HeroInstance();
	h->name = HeroName::CHRISTIAN;
	h->instanceName = "Christian";
	h->typeName = "";
	h->subType = HeroClass::KNIGHT;
	h->subTypeName = "Knight";
	h->initObj();
	this->allHeroes.push_back(h);

	h = new HeroInstance();
	h->name = HeroName::RION;
	h->instanceName = "Rion";
	h->typeName = "Hero";
	h->subType = HeroClass::CLERIC;
	h->subTypeName = "Cleric";
	h->initObj();
	this->allHeroes.push_back(h);

	h = new HeroInstance();
	h->name = HeroName::CAITLIN;
	h->instanceName = "Caitlin";
	h->typeName = "Hero";
	h->subType = HeroClass::CLERIC;
	h->subTypeName = "Cleric";
	h->initObj();
	this->allHeroes.push_back(h);

	h = new HeroInstance();
	h->name = HeroName::LOYNIS;
	h->instanceName = "Loynis";
	h->typeName = "Hero";
	h->subType = HeroClass::CLERIC;
	h->subTypeName = "Cleric";
	h->initObj();
	this->allHeroes.push_back(h);
}

void World::initKingdoms()
{
	vec_kingdoms.resize(2);
	vec_kingdoms[0] = new Kingdom();
	vec_kingdoms[1] = new Kingdom();
	vec_kingdoms[0]->init(Color::RED);
	vec_kingdoms[1]->init(Color::BLUE);
}

void World::initFogOfWar()
{
}


void World::ClearFog(int color)
{
	// vec_castles.scoute(color)

	//vec_heroes.scoute(color)
	
	//mapcapture_obj.scoute(color)
}


void World::Reset()
{
	this->width = 0;
	this->height = 0;
	this->vec_tiles.clear();
	this->vec_kingdoms.clear();
	this->vec_objects.clear();
	this->vec_heros.clear();
	this->vec_town.clear();
	this->allHeroes.clear();
	
	/*for(auto obj : this->vec_objects)
		removeMapObject(obj);*/
}

void World::NewMaps(int sw, int sh)
{
	this->Reset();
	this->initKingdoms();
	this->initAllHeroes();

	this->width = sw;
	this->height = sh;

	this->vec_tiles.resize(width*height);
	int temp = 0;
	for (auto &i : this->vec_tiles)
	{
		i.Init(temp);
		if (temp > 3000)
			i.SetGround(Maps::Ground::GRASS);
		else
			i.SetGround(Maps::Ground::DIRT);
		temp++;
	}
}

void World::sortObjects()
{
	// mozna to zrobic optymalniej
	std::sort(vec_objects.begin(),vec_objects.end(),
		[](MP2::ObjectInstance* a, MP2::ObjectInstance* b)
	{
		return a->pos.y < b->pos.y ||
			(a->pos.y == b->pos.y && a->priority < b->priority);
	});
}

void World::addBlockVisTiles(MP2::ObjectInstance * obj)
{
	for (int y = 0; y < obj->getHeight(); y++)
		for (int x = 0; x < obj->getWidth(); x++)
		{
			int xVal = obj->pos.x - x;
			int yVal = obj->pos.y - y;
			if (xVal >= 0 && xVal < this->width && yVal >= 0 && yVal < this->height)
			{
				Maps::Tile & tile = this->GetTile(xVal, yVal);
				if (obj->blockingAt(xVal, yVal))
				{
					tile.objects.push_back(obj);
					tile.blocked = true;
					tile.SetObjectType(obj->type);
				}
				if (obj->visitableAt(xVal, yVal))
				{
					tile.objects.push_back(obj);
					tile.visitable = true;
					if (obj->blockVisit) 
						tile.blocked = true;
					tile.SetObjectType(obj->type);
				}
			}
		}
}

void World::removeBlockVisTiles(MP2::ObjectInstance * obj, bool total)
{
	for (int y = 0; y < obj->getHeight(); y++)
		for (int x = 0; x < obj->getWidth(); x++)
		{
			int xVal = obj->pos.x - x;
			int yVal = obj->pos.y - y;
			if (xVal >= 0 && xVal < this->width && yVal >= 0 && yVal < this->height)
			{
				Maps::Tile & tile = this->GetTile(xVal, yVal);
				if (total || obj->blockingAt(xVal,yVal))
				{
					tile.removeMapObject(obj);
					tile.blocked = tile.objects.size();
				}
				if (total || obj->visitableAt(xVal, yVal))
				{
					tile.removeMapObject(obj);
					tile.visitable = tile.objects.size();
					tile.blocked = tile.objects.size();;
				}
			}
		}
}

void World::addMapObject(MP2::ObjectInstance * obj)
{
	// wczesniej powinno byc sprawdzone czy obiekt nie wychodzi poza mape
	auto mapObj = std::find(vec_objects.begin(), vec_objects.end(), obj);
	if (mapObj == this->vec_objects.end())
	{
		this->vec_objects.push_back(obj);
		obj->afterAddToMap();

		this->addBlockVisTiles(obj);

		this->sortObjects();
	}
	else
		std::cout << "Object is already existing in world" << std::endl;
}

void World::removeMapObject(MP2::ObjectInstance * obj)
{
	if (obj)
	{
		obj->toDelete = true;
		this->removeBlockVisTiles(obj, false);

		if (obj->type == Obj::HERO)
			for (int i = 0; i < vec_heros.size(); i++)
				if (obj->id = vec_heros[i]->id)
					vec_heros.erase(vec_heros.begin() + i);
		for (int i = 0; i < vec_objects.size(); i++)
			if (obj->id == vec_objects[i]->id)
				vec_objects.erase(vec_objects.begin() + i);
	}
}

void World::removeMapObject(sf::Vector2i pos)
{
	MP2::ObjectInstance* obj = this->GetTile(pos.x, pos.y).GetObjectPtr();
	this->removeMapObject(obj);
}

void World::changeObjPos(int id, sf::Vector2i oldPos,sf::Vector2i newPos)
{
	if (oldPos == newPos) return;
	for (auto obj : this->vec_objects)
		if (obj->id == id)
		{
			this->removeBlockVisTiles(obj, false);
			obj->setTilePos(newPos);
			this->addBlockVisTiles(obj);
		}
	this->sortObjects();
}

void World::revealTilesInRange(sf::Vector2i pos, int radious, int playerColor)
{

	for (int xd = std::max<int>(pos.x - radious, 0); xd <= std::min<int>(pos.x + radious, world.w() - 1); xd++)
	{
		for (int yd = std::max<int>(pos.y - radious, 0); yd <= std::min<int>(pos.y + radious, world.h() - 1); yd++)
		{
			sf::Vector2i tilePos(xd, yd);
			double distance = sqrt(
				(pos.x - tilePos.x) * (pos.x - tilePos.x) +
				(pos.y - tilePos.y) * (pos.y - tilePos.y));

			if (distance <= radious)
			{
				auto &tile = world.GetTile(tilePos);
				tile.ClearFog(playerColor);
			}
		}
	}
}

World::~World()
{
}

bool World::canMoveBetween(const sf::Vector2i & src, const sf::Vector2i & dst) const
{
	const Maps::Tile * dstTile = &world.GetTile(dst);
	const Maps::Tile * srcTile = &world.GetTile(src);
	int dir = Direction::Get(dstTile->GetIndex(), srcTile->GetIndex());
	if (dstTile->GetObjectPtr())
	{
		if(!dstTile->objects.front()->isVisitableFrom(dir))
			return false;
	}
	if (srcTile->GetObjectPtr())
	{
		if (!srcTile->objects.front()->isVisitableFrom(Direction::reflect(dir)))
			return false;
	}
	return true;
}

bool World::load(std::string path)
{
	std::fstream file;
	file.open(path);
	// load kingdoms
	file >> width >> height;
	if (width < 1 || width > 1000 || height < 1 || height > 1000)
	{
		std::cout << "loading erorr" << std::endl;
		file.close();
		return false;
	}

	int temp = 0;
	int type = Obj::NO_OBJ;
	int name;
	vec_tiles.resize(width*height);
	for (auto &tile : this->vec_tiles)
	{
		tile.load(file);
	}
	this->initAllHeroes();
	this->initKingdoms();
	file >> MP2::ObjectInstance::getCountID();
	file >> temp;
	for (int i = 0; i < temp; i++)
	{
		MP2::ObjectInstance * obj;
		file >> type;
		switch (Obj(type))
		{
		case Obj::HERO:
		{
			file >> name;
			obj = world.getHeroByName(HeroName(name));
			break;
		}
		case Obj::TOWN:
		{
			obj = new TownInstance();
			break;
		}
		case Obj::MONSTER:
		{
			obj = new MP2::ObjectInstance();
			break;
		}
		default:
			obj = new MP2::ObjectInstance();
			break;
		}
		obj->load(file);
		this->addMapObject(obj);
	}
	for (auto town : this->vec_town)
		town->setObjectsToBuildings();
	return true;
}

void World::save(std::string path)
{
	std::fstream file;
	file.open(path);
	file.clear();
	file << world.w() << " " << world.h() << std::endl;

	for (auto tile : vec_tiles)
	{
		tile.save(file);
	}
	file << std::endl;
	file << MP2::ObjectInstance::getCountID() << " ";
	file << vec_objects.size() << std::endl;
	for (auto obj : vec_objects)
	{
		file << obj->type << " ";
		obj->save(file);
	}
	file.close();
}