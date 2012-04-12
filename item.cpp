#include "item.hpp"

Item::Item(void) {
    this->type = Item::GENERIC;
}

void
Item::SetPosition(int x, int y) {
    this->pos.x = x;
    this->pos.y = y;
}
