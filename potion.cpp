#include "potion.hpp"
#include <sstream>

static const char* PRELUDE = "Potion of ";
static const char* POTENCIES[Potion::LAST_POTENCY] = {
    "Minor",
    "Lesser",
    "Light",
    "Moderate",
    "Average",
    "Strong",
};
static const char* CATEGORIES[Potion::LAST_CATEGORY] = {
    "Healing",
    "Enervation",
    "Rejuvenation",
};

Potion::Potion(Potion::Potency inPot, Potion::Category inCat) {
    this->potency = inPot;
    this->category = inCat;
    this->type = Item::POTION;
    this->symbol = 'p';

    switch (this->category) {
        case Potion::HEALING:
            this->colour = COL_RED;
            break;
        case Potion::ENERVATION:
            this->colour = COL_BLUE;
            break;
        case Potion::REJUVENATION:
            this->colour = COL_PURPLE;
            break;
        case Potion::LAST_CATEGORY:
            this->colour = COL_WHITE;
    }
}

void
Potion::ApplyEffects(Character *c) {
    std::stringstream ss;
    int effect;

    //Healing
    if (this->category == HEALING || this->category == REJUVENATION) {
        effect = 6;
        for (int i = (int) this->potency; i >= 0; --i)
            effect += rand() % 12;
        c->Heal(effect);
    }

    //Mana restoration
    if (this->category == ENERVATION || this->category == REJUVENATION) {
        effect = 6;
        for (int i = (int) this->potency; i >= 0; --i)
            effect += rand() % 12;
        c->RecoverMP(effect);
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
