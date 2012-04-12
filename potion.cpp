#include "potion.hpp"

static const char *PotionNames[Potion::LAST_CATEGORY][Potion::LAST_STRENGTH] {
    {"Watery healing",      "Minor healing",        "Light healing",        "Moderate Healing"},
    {"Watery enervation",   "Minor enervation",     "Light enervation",     "Moderate enervation"},
    {"Watery rejuvenation", "Minor rejuvenation",   "Light rejuvenation",   "Moderate rejuvenation"},
};

Potion::Potion(Potion::Strength inStr, Potion::Category inCat) {
    this->name = PotionNames[inCat][inStr];
    this->strength = inStr;
    this->type = Item::POTION;
    this->symbol = 'p';

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
