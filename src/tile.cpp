#include "tile.hpp"

Tile::Tile(void) {}

Tile::Tile(bool vis, char inC) {
    this->isVisible = vis;
    this->c = inC;
}

bool
Tile::SeeThrough(void) {
    if (this->c == WALL_CHAR)
        return false;
    if (this->c == CLOSED_DOOR_CHAR)
        return false;
    if (this->c == PILLAR_CHAR)
        return false;
    return true;
}
