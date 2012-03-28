#include "level.hpp"
#include <cstdlib>

Level::Level(Level* parent) {
    this->prev = parent;
    this->next = NULL;

    if (this->prev)
        this->depth = this->prev->depth + 1;
    else
        this->depth = 1;

    for (int e = 0; e < MAP_H; ++e) {
        for (int i = 0; i < MAP_W; ++i) {
            if (rand() % 5) 
                this->tiles[i][e] = Tile(true, '#');
            else 
                this->tiles[i][e] = Tile(true, ' ');
        }
    }
}
