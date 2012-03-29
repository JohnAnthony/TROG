#include "level.hpp"
#include <cstdlib>

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

    r.w = (rand() % 7) + 5;
    r.h = (rand() % 7) + 5;
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
    //Floor
    for (int e = 0; e < r->h; ++e) {
        for (int i = 0; i < r->w; ++i) {
            this->tiles[ i + r->x ][ e + r->y ].c = FLOOR_CHAR;
        }
    }
}

void
Level::ApplyCorridor(Room *c) {
}
