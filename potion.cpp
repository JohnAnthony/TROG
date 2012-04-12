#include "potion.hpp"
#include <sstream>

static const char* prelude = "Potion of ";

Potion::Potion(Potion::Potency inPot, Potion::Category inCat) {
    this->potency = inPot;
    this->type = Item::POTION;
    this->symbol = 'p';

    //Category
    if (this->category == Potion::HEALING)
        this->colour = COL_RED;
    else if (this->category == Potion::ENERVATION)
        this->colour = COL_BLUE;
    else if (this->category == Potion::REJUVENATION)
        this->colour = COL_PURPLE;
}

void
Potion::ApplyEffects(Character *c) {

}

std::string
Potion::GetName(void) {
    std::stringstream ss;
    ss << prelude;
    return ss.str();
}
