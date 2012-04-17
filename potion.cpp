#include "potion.hpp"
#include <sstream>

typedef struct {
    char const * const colour;
    char const * const smell;
} PotionDescription;

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

static PotionDescription DESCRIPTIONS[Potion::LAST_CATEGORY] = {
    {"red", "broth"},              //Healing
    {"blue", "cinnamon"},          //Enervation
    {"purple", "liquorice"},       //Rejuvenation
};

Potion::Potion(Potion::Potency inPot, Potion::Category inCat) {
    this->potency = inPot;
    this->category = inCat;
    this->type = Item::POTION;
    this->symbol = 'p';

    if (this->potency >= Potion::LAST_POTENCY)
        this->potency = (Potion::Potency)((int)Potion::LAST_POTENCY - 1);

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
        effect = 10;
        effect += DICEROLL(potency + 1, 20);
        c->Heal(effect);
    }

    //Mana restoration
    if (this->category == ENERVATION || this->category == REJUVENATION) {
        effect = 10;
        effect += DICEROLL(potency + 1, 20);
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

std::string
Potion::GetLongDescription(void) {
    std::stringstream ss;
    PotionDescription *ds;

    ds = &DESCRIPTIONS[this->category];
    ss << "A vial of " << ds->colour << " liquid that smells of " << ds->smell;

    return ss.str();
}

Potion::~Potion(void) {
}
