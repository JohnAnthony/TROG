#include "potion.hpp"
#include <sstream>

static const char* PRELUDE = "Potion of ";
static const char* POTENCIES[Potion::LAST_POTENCY] = {
    "Minor",
    "Lesser",
    "Light",
    "Moderate",
    "Average"
};
static const char* CATEGORIES[Potion::LAST_CATEGORY] = {
    "Healing",
    "Enervation",
    "Rejuvenation",
    "Mystery"
};

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
    int effect;

    //Healing
    if (this->category == HEALING || this->category == REJUVENATION) {

    }

    //Mana restoration
    if (this->category == ENERVATION || this->category == REJUVENATION) {

    }
}

std::string
Potion::GetName(void) {
    std::stringstream ss;
    ss << PRELUDE;
    ss << POTENCIES[this->potency];
    ss << ' ';
    ss << CATEGORIES[this->category];
    return ss.str();
}
