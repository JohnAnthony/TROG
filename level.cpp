#include "level.hpp"
#include <cstdlib>

#define CORRIDOR_TRIES      5
#define ROOM_TRIES          5

Level::Level(Level* parent) {
    Room r;

    this->prev = parent;
    this->next = NULL;

    if (this->prev)
        this->depth = this->prev->depth + 1;
    else
        this->depth = 1;

    //Blank everything
    for (int e = 0; e < MAP_H; ++e) {
        for (int i = 0; i < MAP_W; ++i) {
            this->tiles[i][e] = Tile(true, WALL_CHAR);
        }
    }

    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;
    r.x = rand() % (MAP_W - r.w - 2);
    r.y = rand() % (MAP_H - r.h - 2);
    this->ApplyRoom(&r);
}

Level::~Level(void) {
    if (this->next)
        delete this->next;
}

void
Level::ApplyRoom(Room *r) {
    Corridor c;

    //Floor
    for (int e = 0; e < r->h; ++e) {
        for (int i = 0; i < r->w; ++i) {
            this->tiles[ i + r->x ][ e + r->y ].c = FLOOR_CHAR;
        }
    }

    for (int i = 0; i < CORRIDOR_TRIES; ++i) {
        c = this->FindRoomCorridorChild(r);
        if (this->CorridorFits(&c))
            this->ApplyCorridor(&c);
    }
}

bool
Level::CorridorFits(Corridor *c) {
    if (c->pos.x <= 1 || c->pos.y <= 1)
        return false;
    if (c->pos.x + c->pos.w >= MAP_W)
        return false;
    if (c->pos.y + c->pos.h >= MAP_H)
        return false;
    return true;
}

void
Level::ApplyCorridor(Corridor *c) {
    this->tiles[c->pos.x][c->pos.y].c = CLOSED_DOOR_CHAR;
    this->tiles[c->pos.x + c->pos.w][c->pos.y + c->pos.h].c = CLOSED_DOOR_CHAR;
    if (c->direction == Direction::EAST || c->direction == Direction::WEST) {
        for (int i = 1; i < c->pos.w; ++i)
            this->tiles[i + c->pos.x ][c->pos.y].c = FLOOR_CHAR;
    }
    else {
        for (int i = 1; i < c->pos.h; ++i)
            this->tiles[c->pos.x][ i + c->pos.y ].c = FLOOR_CHAR;
    }
}

Corridor
Level::FindRoomCorridorChild(Room *r) {
    Corridor c;

    if (rand() % 2) { //NORTH OR SOUTH
        c.pos.w = 0;
        c.pos.h = rand() % 5 + 3;
        c.pos.x = r->x + rand() % r->w;
        if (rand() % 2) { //NORTH
            c.direction = Direction::NORTH;
            c.pos.y = r->y - 1 - c.pos.h;
        }
        else { //SOUTH
            c.direction = Direction::SOUTH;
            c.pos.y = r->y + r->h;
        }
    }
    else { //EAST OR WEST
        c.pos.h = 0;
        c.pos.w = rand() % 5 + 3;
        c.pos.y = r->y + rand() % r->h;
        if (rand() % 2) { // WEST
            c.direction = Direction::WEST;
            c.pos.x = r->x - 1 - c.pos.w;
        }
        else { //EAST
            c.direction = Direction::EAST;
            c.pos.x = r->x + r->w;
        }
    }

    return c;
}
