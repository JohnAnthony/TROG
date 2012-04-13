#include "stattome.hpp"
#include "character.hpp"
#include "gui.hpp"
#include <sstream>

static char const * const CategoryTexts[StatTome::LAST_CATEGORY] = {
    "bodily vigour",
    "enervation",
    "bodily strength",
    "rhino's hide",
    "the swordsman",
    "the acrobat",
    "sorcerer's might",
    "dwarven courage",
    "higher learning"
};

StatTome::StatTome(unsigned int pot) {
    this->potency = rand() % pot + 5;
    this->type = Item::STAT_TOME;
    this->category = (StatTome::Category)(rand() % StatTome::LAST_CATEGORY);
    this->symbol = 'B';
    this->colour = COL_RED;
}

std::string
StatTome::GetName(void) {
    std::stringstream ss;
    ss << "A ";

    if (this->potency < 5)
        ss << "book";
    else if (this->potency < 10)
        ss << "grand book";
    else if (this->potency < 15)
        ss << "tome";
    else if (this->potency < 20)
        ss << "grand tome";
    else if (this->potency < 25)
        ss << "grimoire";
    else
        ss << "grand grimoire";
    
    ss << " of " << CategoryTexts[this->category];
    return ss.str();
}

void
StatTome::ApplyEffects(Character *c) {
    GUI::Alert("Something happened.");
}
