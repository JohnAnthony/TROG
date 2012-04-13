#include "item.hpp"
#include "potion.hpp"
#include "treasure.hpp"
#include "stattome.hpp"

Item::Item(void) {
    this->type = Item::GENERIC;
}

void
Item::SetPosition(int x, int y) {
    this->pos.x = x;
    this->pos.y = y;
}

std::string
Item::GetName(void) {
    if (this->type == Item::POTION)
        return ((Potion*)this)->GetName();
    else if (this->type == Item::TREASURE_T)
        return ((Treasure*)this)->GetName();
    else if (this->type == Item::STAT_TOME)
        return ((StatTome*)this)->GetName();
    else
        return "Unknown";
}
