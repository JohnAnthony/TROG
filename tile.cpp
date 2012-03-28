#include "tile.hpp"

Tile::Tile(void) {}

Tile::Tile(bool vis, char inC) {
    this->isVisible = vis;
    this->c = inC;
}
