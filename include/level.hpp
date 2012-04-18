#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <list>
#include "tile.hpp"
#include "geometry.hpp"
#include "character.hpp"
#include "game.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "treasure.hpp"

#define MAP_W   100
#define MAP_H   50

typedef Rect Room;

class Game;

struct Corridor {
    Rect pos;
    Direction::Type direction;
};

struct RoomText {
    Rect pos;
    char const * const * text;
    bool seen;
};

class Level {
    public:
    Level(Level* parent);
    ~Level(void);
    bool TileIsVisible(Point p);
    void ConditionallyShowObject(Point p, char c, int col);
    void RevealSight(Point p, int distance);
    void DoSightBeam(Direction::Type d, int x, int y, float ttl);
    void EnemySpawn(Rect *r);
    void RemoveEnemy(Enemy *e);
    std::string DescriptionOfTile(Point p, Game *g);
    void GiveEnemiesTurn(Character *c);
    void EnemyAdvance(Enemy *e, Character *c);
    Enemy* GetEnemy(Point p);
    Item* GetItem(Point p);
    void AddPotion(Rect *r);
    void CheckForRoomText(Character *c);
    void AddEquippable(Rect *r);
    void RevealAll(void);
    bool CanSee(Point p1, Point p2);
    void ClearObstacles(void);

    unsigned int        depth;
    Tile                tiles[MAP_W][MAP_H];
    Level*              prev;
    Level*              next;
    Point               stairs_up;
    Point               stairs_down;
    std::list<Item*>    items;
    std::list<Enemy>    enemies;
    std::vector<RoomText> roomtexts;
    unsigned int        maximal_enemy;

    private:
    void ApplyRoom(Room * const r, bool isFirstRoom);
    void ApplyCorridor(Corridor const * const c);
    bool CorridorFits(Corridor const * const c);
    bool RoomFits(Room const * const r);
    Corridor FindRoomCorridorChild(Room *r);
    Room RoomFromCorridor(Corridor *c);
    void AddGold(Rect *r);
    void MakeSpecialRoom(Rect *r);
    void AddPillars(Rect *r);
    void AddRoomText(Rect *r, char const * const * const msg);
};

#endif
