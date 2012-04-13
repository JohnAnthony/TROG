#include "stattome.hpp"

StatTome::StatTome(unsigned int pot) {
    this->potency = pot;
    this->type = Item::STAT_TOME;
    this->symbol = 'B';
    this->colour = COL_RED;
}

std::string
StatTome::GetName(void) {
    return "A mysterious tome.";
}
