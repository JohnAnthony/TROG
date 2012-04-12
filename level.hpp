#ifndef LEVEL_H
#define LEVEL_H

#include <list>
#include "tile.hpp"
#include "geometry.hpp"
#include "character.hpp"
#include "game.hpp"
#include "item.hpp"
#include "enemy.hpp"

#define MAP_W   100
#define MAP_H   50

typedef Rect Room;

class Game;

struct Corridor {
    Rect pos;
    Direction::Type direction;
};

struct GoldPile {
    Point pos;
    int quantity;
};

class Level {
    public:
    Level(Level* parent);
    ~Level(void);
    void Draw(Game *g);
    void DrawObjectRelative(Point p, char c);
    bool IsOnScreen(Point p);
    bool TileIsVisible(Point p);
    void ConditionallyShowObject(Point p, char c, int col);
    void RevealSight(Entity* e);
    void CentreCam(Point p);
    void DoSightBeam(Direction::Type d, int x, int y, float ttl);
    void EnemySpawn(Rect *r);
    void RemoveEnemy(Enemy *e);
    std::string DescriptionOfTile(Point p, Game *g);
    void GiveEnemiesTurn(Character *c);
    void EnemyAdvance(Enemy *e, Character *c);
    Enemy* GetEnemy(Point p);

    int                 depth;
    Tile                tiles[MAP_W][MAP_H];
    Level*              prev;
    Level*              next;
    Point               stairs_up;
    Point               stairs_down;
    std::list<GoldPile> goldpiles;
    std::list<Enemy>    enemies;

    static Point        cam;
    static Character    *character;

    private:
    void ApplyRoom(Room * const r);
    void ApplyCorridor(Corridor const * const c);
    bool CorridorFits(Corridor const * const c);
    bool RoomFits(Room const * const r);
    Corridor FindRoomCorridorChild(Room *r);
    Room RoomFromCorridor(Corridor *c);
    void AddGold(Rect *r);
};

#endif
