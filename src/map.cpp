#include "map.hpp"
#include "game.hpp"

using namespace Mario;

Map::Map(Game* game, std::string path)
{
    this->game = game;
    this->player = nullptr;
    this->edit_mode = false;

    Load(path);
    SpawnObjects();

    log_info("[Map]\tCreated.");
}

Map::~Map()
{
    delete player;

    log_info("[Map]\tDeleted.");
}

void Map::Load(std::string path)
{
    tiles.clear();

    std::fstream file(path, std::ios::in);
    std::string line;

    while (true)
    {
        std::getline(file, line);

        if (file.eof())
            break;

        unsigned raw;
        TileRow row;
        std::stringstream row_stream(line);

        while (row_stream >> raw)
            row.push_back(Tile(raw));

        tiles.push_back(row);
    }

    file.close();
    log_info("[Map]\tLoaded.");
}

void Map::Save(std::string path)
{
    std::fstream file(path, std::ios::out);

    for (TileSet::iterator c = tiles.begin(); c != tiles.end(); ++c)
    {
        for (TileRow::iterator r = c->begin(); r != c->end(); ++r)
            file << Tile(*r) << " ";

        file << std::endl;
    }

    file.close();
    log_info("[Map]\tSaved.");
}

void Map::AddObject(Object* o)
{
    objs.push_back(o);

    game->OnObjectAddToMap(o);
}

void Map::AddPlayer(Player* p)
{
    objs.push_back(p);

    game->OnPlayerAddToMap(p);
}

void Map::SpawnObjects()
{
    for (size_t x = 0; x < tiles.size(); ++x)
        for (size_t y = 0; y < tiles[x].size(); ++y)
            switch (tiles[x][y])
            {
                case TILE_PLAYER_SPAWN:
                    AddPlayer(new Player(x, y));
                    break;

                case TILE_GOOMBA_SPAWN:
                    AddObject(new Goomba(x, y));
                    break;

                case TILE_KOOPA_SPAWN:
                    AddObject(new Koopa(x, y));
                    break;

                case TILE_LAKITU_SPAWN:
                    AddObject(new Lakitu(x, y));
                    break;

                case TILE_SPINY_SPAWN:
                    AddObject(new Spiny(x, y));
                    break;
            }
}

inline Tile Map::GetTile(int tile_x, int tile_y)
{
    if (tile_x < 0 || tile_y < 0 ||
        size_t(tile_x) >= tiles.size() ||
        size_t(tile_y) >= tiles[size_t(tile_x)].size())
        return TILE_EMPTY;

    return Tile(tiles[size_t(tile_x)][size_t(tile_y)]);
}

inline Tile Map::GetTileAtPos(float x, float y)
{
    return GetTile(ToTile(x), ToTile(y));
}

inline bool Map::IsSolidTile(Tile tile)
{
    return tile > TILE_EMPTY && tile < TILE_PLAYER_SPAWN;
}

void Map::Update(float dt)
{
    for (std::list<Object*>::iterator i = objs.begin(); i != objs.end(); ++i)
    {
        (*i)->OnUpdate(dt);

        if ((*i)->falling && IsSolidTile( GetTileAtPos((*i)->pos_x + (*i)->dir_x * dt, (*i)->pos_y + (*i)->dir_y * dt) ))
        {
            (*i)->falling = false;
            (*i)->pos_y = floor((*i)->pos_y/TileSize)*TileSize;
            (*i)->dir_y = 0;
        }

        else if (!(*i)->falling && !IsSolidTile(GetTileAtPos((*i)->pos_x + (*i)->dir_x * dt, (*i)->pos_y + (*i)->dir_y * dt - 1)))
            (*i)->falling = true;
    }
}
