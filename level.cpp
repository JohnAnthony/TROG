#include "level.hpp"
#include <cstdlib>

#define CORRIDOR_TRIES      30

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
    r.x = rand() % (MAP_W - r.w - 2) + 1;
    r.y = rand() % (MAP_H - r.h - 2) + 1;
    this->ApplyRoom(&r);
}

Level::~Level(void) {
    if (this->next)
        delete this->next;
}

void
Level::ApplyRoom(Room *r) {
    Corridor c;
    Room r_child;

    //Floor
    for (int e = 0; e < r->h; ++e) {
        for (int i = 0; i < r->w; ++i) {
            this->tiles[ i + r->x ][ e + r->y ].c = FLOOR_CHAR;
        }
    }

    for (int i = 0; i < CORRIDOR_TRIES; ++i) {
        c = this->FindRoomCorridorChild(r);
        if (!this->CorridorFits(&c))
            continue;
        r_child = this->RoomFromCorridor(&c);
        if (!this->RoomFits(&r_child))
            continue;
        this->ApplyCorridor(&c);
        this->ApplyRoom(&r_child);
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

bool
Level::RoomFits(Room *r) {
    if (r->x <= 0 || r->y <= 0)
        return false;
    if (r->x + r->w >= MAP_W)
        return false;
    if (r->y + r->h >= MAP_H)
        return false;

    for (int e = MAX(r->y - 1, 0); e <= r->y + r->h; ++e) {
        for (int i = MAX(r->x - 1, 0); i <= r->x + r->w; ++i) {
            if (this->tiles[i][e].c == FLOOR_CHAR)
                return false;
        }
    }

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

Room
Level::RoomFromCorridor(Corridor *c) {
    Room r;
    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;

    // Only add-sized rooms
    if (r.w % 2 == 0)
        r.w++;
    if (r.h % 2 == 0)
        r.h++;

    if (c->direction == Direction::NORTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y - r.h;
    }
    else if (c->direction == Direction::SOUTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y + c->pos.h + 1;
    }
    else if (c->direction == Direction::EAST) {
        r.x = c->pos.x + c->pos.w + 1;
        r.y = c->pos.y - rand() % r.h;
    }
    else { //WEST
        r.x = c->pos.x - r.w;
        r.y = c->pos.y - rand() % r.h;
    }

    return r;
}

Corridor
Level::FindRoomCorridorChild(Room *r) {
    Corridor c;

    if (rand() % 2) { //NORTH OR SOUTH
        c.pos.w = 0;
        c.pos.h = ((rand() % 2) + 1) * 3;
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
        c.pos.w = ((rand() % 3) + 1) * 3;
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
