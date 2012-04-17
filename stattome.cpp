#include "stattome.hpp"
#include "character.hpp"
#include "gui.hpp"
#include <sstream>

typedef struct {
    char const * const name;
    char const * const cover;
} StatTomeDescription;

static StatTomeDescription CategoryTexts[StatTome::LAST_CATEGORY] = {
    {"bodily vigour", "A red book with an oliphant on the cover"},
    {"enervation", "A blue book whose cover depicts a raksha being decapitated"},
    {"bodily strength", "A red book whose cover depicts a man holding open a portcullis"},
    {"rhino's hide", "A book bound in what appears to be worked grey leather"},
    {"the swordsman", "A plain grey book with a steel spine"},
    {"the acrobat", "A book whose cover is a blue-and-white sketch of acrobats in motion"},
    {"sorcerer's might", "A black book whose cover bears the symbol of magic in stark white"},
    {"dwarven courage", "A ruddy red-orange book whose cover depicts dwarves standing against a dragon"},
    {"higher learning", "A green book whose cover depicts, in surgical detail, a cross-sectioned brain"}
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

    if (this->potency < 10)
        ss << "book";
    else if (this->potency < 15)
        ss << "grand book";
    else if (this->potency < 20)
        ss << "tome";
    else if (this->potency < 25)
        ss << "grand tome";
    else if (this->potency < 30)
        ss << "grimoire";
    else
        ss << "grand grimoire";
    
    ss << " of " << CategoryTexts[this->category].name;
    return ss.str();
}

void
StatTome::ApplyEffects(Character *c) {
    std::stringstream ss;
    int effect;

    GUI::Alert("You turn the pages of the book over carefully...");

    // Chance of a curse
    if (rand() % (100 + c->curMAG + c->curWIL) < 10 + (int)this->potency) {
        GUI::Alert("Something went wrong!");
        c->RandomCurse(this->potency);
        return;
    }

    effect = rand() % this->potency + 1;
    ss << "The book's magic empowers you. Your ";

    switch(this->category) {
        case StatTome::HEALTH:
            c->baseHP += effect;
            c->curHP += effect;
            ss << "health";
            break;
        case StatTome::ENERGY:
            c->baseMP += effect;
            c->curMP += effect;
            ss << "magical energy";
            break;
        case StatTome::STRENGTH:
            c->baseSTR += effect;
            c->curSTR += effect;
            ss << "strength";
            break;
        case StatTome::TOUGHNESS:
            c->baseTOU += effect;
            c->curTOU += effect;
            ss << "toughness";
            break;
        case StatTome::SKILL:
            c->baseATT += effect;
            c->curATT += effect;
            ss << "attack skill";
            break;
        case StatTome::DODGE:
            c->baseDEF += effect;
            c->curDEF += effect;
            ss << "defensive skill";
            break;
        case StatTome::MAGERY:
            c->baseMAG += effect;
            c->curMAG += effect;
            ss << "sorcerous power";
            break;
        case StatTome::WILL:
            c->baseWIL += effect;
            c->curWIL += effect;
            ss << "willpower";
            break;
        case StatTome::XP:
            effect *= 10;
            c->GiveXP(effect);
            ss << "experience";
            break;
        case StatTome::LAST_CATEGORY:
        default:
            break;
    }

    ss << " has increased by " << effect << " !";

    GUI::Alert(ss.str());
}

std::string
StatTome::GetLongDescription(void) {
    std::stringstream ss;

    ss << CategoryTexts[this->category].cover;

    return ss.str();
}
