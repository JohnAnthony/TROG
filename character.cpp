#include "character.hpp"

static Affinity ClassAffinities[Character::LAST_CLASS] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {10,    0,  5,  5,  5,  5,  0,  5,  0,  0},  //FIGHTER
    {6,     0,  2,  2,  2,  2,  0,  5,  0,  0},  //THIEF
    {4,     10, 1,  1,  1,  1,  15, 7,  0,  0},  //WIZARD
    {6,     6,  3,  3,  3,  3,  13, 8,  0,  0},  //CLERIC
    {12,    0,  7,  7,  4,  4,  0,  4,  0,  0},  //BARBARIAN
    {10,    4,  4,  4,  4,  4,  8,  7,  0,  0},  //PALADIN
    {6,     6,  2,  2,  2,  2,  13, 6,  0,  0},  //DRUID
    {4,     8,  1,  1,  1,  1,  12, 7,  0,  0},  //SAGE
};
static Affinity RaceAffinities[Character::LAST_RACE] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {10,    0,  5,  5,  5,  5,  0,  5,  0,  5},  //HUMAN
    {5,     2,  4,  3,  7,  6,  1,  5,  -5, 7},  //ELF
    {15,    0,  5,  8,  5,  5,  -2, 10, 5,  6},  //DWARF
    {8,     0,  5,  4,  5,  5,  0,  5,  -2, 6},  //HALF_ELF
    {10,    0,  3,  3,  6,  8,  0,  5,  2,  5},  //HALFLING
    {10,    0,  7,  6,  5,  5,  0,  5,  0,  6},  //HALF_ORC
    {10,    0,  8,  7,  5,  5,  0,  5,  0,  7},  //ORC
    {10,    0,  6,  6,  5,  5,  0,  5,  0,  6},  //LIZARDFOLK
};

Character::Character(std::string inName, Character::Race inRace, Character::Class inClass) {
    this->name = inName;
    this->cclass = inClass;
    this->race = inRace;
    this->XP = 0;
    this->Level = 1;
    this->symbol = '@';
    this->colour = COL_GREEN;
    this->affinity = this->SumAffinities(inRace, inClass);
    this->next_level = 1000;

    this->maxHP  = this->affinity.hp;
    this->maxMP  = this->affinity.mp;
    this->maxSTR = this->affinity.str;
    this->maxTOU = this->affinity.tou;
    this->maxATT = this->affinity.att;
    this->maxDEF = this->affinity.def;
    this->maxMAG = this->affinity.mag;
    this->maxWIL = this->affinity.wil;

    //You come into this world naked and screaming
    this->helm = NULL;
    this->armour = NULL;
    this->weapon = NULL;
    this->gloves = NULL;
    this->boots = NULL;
    this->ring1 = NULL;
    this->ring2 = NULL;
    this->neck = NULL;

    this->sight_range = this->affinity.sight;
    this->mv_cost = 1000 + this->affinity.mv;

    this->FullyRestore();
}

void
Character::FullyRestore(void) {
    this->curHP = this->maxHP;
    this->curMP = this->maxMP;
    this->curSTR = this->maxSTR;
    this->curTOU = this->maxTOU;
    this->curATT = this->maxATT;
    this->curDEF = this->maxDEF;
    this->curMAG = this->maxMAG;
    this->curWIL = this->maxWIL;
}

void
Character::LevelUp(void) {
    this->Level++;
    if (this->affinity.hp > 0)
        this->maxHP += rand() % this->affinity.hp;
    if (this->affinity.mp > 0)
        this->maxMP += rand() % this->affinity.mp;
    if (this->affinity.str > 0)
        this->maxSTR += rand() % this->affinity.str;
    if (this->affinity.tou > 0)
        this->maxTOU += rand() % this->affinity.tou;
    if (this->affinity.att > 0)
        this->maxATT += rand() % this->affinity.att;
    if (this->affinity.def > 0)
        this->maxDEF += rand() % this->affinity.def;
    if (this->affinity.mag > 0)
        this->maxMAG += rand() % this->affinity.mag;
    if (this->affinity.wil > 0)
        this->maxWIL += rand() % this->affinity.wil;

    this->next_level = this->Level * this->Level * 1000;
    this->FullyRestore();
}

void
Character::MoveTo(Point p) {
    this->pos = p;
}

std::string
Character::ClassString(void) {
    switch (this->cclass) {
        case Character::FIGHTER:
            return "FIGHTER";
        case Character::THIEF:
            return "THIEF";
        case Character::WIZARD:
            return "WIZARD";
        case Character::CLERIC:
            return "CLERIC";
        case Character::BARBARIAN:
            return "BARBARIAN";
        case Character::PALADIN:
            return "PALADIN";
        case Character::DRUID:
            return "DRUID";
        case Character::SAGE:
            return "SAGE";
        default:
            return "UNKNOWN_CLASS";
    }
}

std::string
Character::RaceString(void) {
    switch (this->race) {
        case Character::HUMAN:
            return "HUMAN";
        case Character::ELF:
            return "ELF";
        case Character::DWARF:
            return "DWARF";
        case Character::HALF_ELF:
            return "HALF_ELF";
        case Character::HALFLING:
            return "HALFLING";
        case Character::HALF_ORC:
            return "HALF_ORC";
        case Character::ORC:
            return "ORC";
        case Character::LIZARDFOLK:
            return "LIZARDFOLK";
        default:
            return "UNKNOWN_RACE";
    }
}

Affinity
Character::SumAffinities(Race inRace, Class inClass) {
    Affinity *a, *b, ret;

    a = &RaceAffinities[inRace];
    b = &ClassAffinities[inClass];

    ret.hp = a->hp + b->hp;
    ret.mp = a->mp + b->mp;
    ret.str = a->str + b->str;
    ret.tou = a->tou + b->tou;
    ret.att = a->att + b->att;
    ret.def = a->def + b->def;
    ret.mag = a->mag + b->mag;
    ret.wil = a->wil + b->wil;
    ret.mv = a->mv + b->mv;
    ret.sight = a->sight + b->sight;

    return ret;
}
