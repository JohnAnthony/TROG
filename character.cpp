#include "character.hpp"
#include "gui.hpp"
#include <sstream>
#include <cstdlib>

static Affinity ClassAffinities[Character::LAST_CLASS] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {12,    0,  7,  7,  4,  4,  0,  4,  0,  0},  //BARBARIAN
    {6,     6,  3,  3,  3,  3,  13, 8,  0,  0},  //CLERIC
    {6,     6,  2,  2,  2,  2,  13, 6,  0,  0},  //DRUID
    {10,    0,  6,  6,  6,  6,  0,  5,  0,  0},  //FIGHTER
    {10,    4,  6,  6,  5,  5,  8,  7,  0,  0},  //PALADIN
    {4,     8,  1,  1,  1,  1,  12, 7,  0,  0},  //SAGE
    {6,     0,  2,  2,  2,  2,  0,  5,  0,  0},  //THIEF
    {4,     10, 1,  1,  1,  1,  15, 7,  0,  0}   //WIZARD
};
static Affinity RaceAffinities[Character::LAST_RACE] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {15,    -5, 5,  8,  5,  5,  -2, 10, 50, 6},  //DWARF
    {5,     2,  4,  3,  7,  6,  1,  5,  -50,7},  //ELF
    {10,    -2, 8,  7,  5,  5,  0,  3,  0,  7},  //GREY ORC
    {10,    0,  3,  3,  6,  8,  0,  5,  20, 5},  //HALFLING
    {8,     0,  5,  4,  5,  5,  0,  5,  -25,6},  //HALF_ELF
    {10,    -1, 7,  6,  5,  5,  0,  4,  0,  6},  //HALF_ORC
    {10,    0,  5,  5,  5,  5,  0,  5,  0,  5},  //HUMAN
    {10,    0,  6,  6,  5,  5,  0,  4,  0,  6}   //LIZARDFOLK
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

    this->maxHP  = this->affinity.hp * 2;
    this->maxMP  = this->affinity.mp * 2;
    this->maxSTR = this->affinity.str * 2;
    this->maxTOU = this->affinity.tou * 2;
    this->maxATT = this->affinity.att * 2;
    this->maxDEF = this->affinity.def * 2;
    this->maxMAG = this->affinity.mag * 2;
    this->maxWIL = this->affinity.wil * 2;

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
    std::stringstream ss;

    this->Level++;
    if (this->affinity.hp > 0)
        this->maxHP += rand() % this->affinity.hp + 1;
    if (this->affinity.mp > 0)
        this->maxMP += rand() % this->affinity.mp + 1;
    if (this->affinity.str > 0)
        this->maxSTR += rand() % this->affinity.str + 1;
    if (this->affinity.tou > 0)
        this->maxTOU += rand() % this->affinity.tou + 1;
    if (this->affinity.att > 0)
        this->maxATT += rand() % this->affinity.att + 1;
    if (this->affinity.def > 0)
        this->maxDEF += rand() % this->affinity.def + 1;
    if (this->affinity.mag > 0)
        this->maxMAG += rand() % this->affinity.mag + 1;
    if (this->affinity.wil > 0)
        this->maxWIL += rand() % this->affinity.wil + 1;
    // Note that mv and sight don't get modified with everythign else

    this->next_level += this->Level * 1000;
    this->FullyRestore();

    ss << this->name << " hits level " << this->Level << "!";
    GUI::Alert(ss.str().c_str());
}

void
Character::MoveTo(Point p) {
    this->pos = p;
}

std::string
Character::GetClassString(void) {
    return this->ClassAsString(this->cclass);
}

std::string
Character::GetRaceString(void) {
    return this->RaceAsString(this->race);
}


std::string
Character::ClassAsString(Character::Class inClass) {
    switch (inClass) {
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
Character::RaceAsString(Character::Race inRace) {
    switch (inRace) {
        case Character::HUMAN:
            return "HUMAN";
        case Character::ELF:
            return "ELF";
        case Character::DWARF:
            return "DWARF";
        case Character::HALF_ELF:
            return "HALF-ELF";
        case Character::HALFLING:
            return "HALFLING";
        case Character::HALF_ORC:
            return "HALF-ORC";
        case Character::GREY_ORC:
            return "GREY ORC";
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

    //Enforce minimums
    if (ret.hp < 1)
        ret.hp = 1;
    if (ret.mp < 0)
        ret.mp = 0;
    if (ret.str < 1)
        ret.str = 1;
    if (ret.tou < 1)
        ret.tou = 1;
    if (ret.att < 1)
        ret.att = 1;
    if (ret.def < 1)
        ret.def = 1;
    if (ret.mag < 1)
        ret.mag = 1;
    if (ret.wil < 1)
        ret.wil = 1;
    if (ret.sight < 1)
        ret.sight = 1;
    //Note that mv doesn't modify liek everythign else

    return ret;
}

void
Character::GiveXP(unsigned int quant) {
    this->XP += quant;
    if (this->XP >= this->next_level)
        this->LevelUp();
}

void
Character::ItemToInventory(Item *i) {
    Inventory.push_back(i);
}

void
Character::DrinkPotion(int n) {
    Item *item;
    Potion *potion;
    std::list<Item*>::iterator it;

    if (n < 0)
        return;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::POTION) {
            if (n == 0)
                break;
            --n;
        }
    }

    if (n != 0) {
        GUI::Alert("Something went wrong trying to find the selected potion...");
        return;
    }

    this->Inventory.erase(it);
    //Now we deal with the effects of the potion
    potion = (Potion*) item;
    potion->ApplyEffects(this);
    delete potion;
}

void
Character::Heal(int n) {
    std::stringstream ss;

    ss << "Recovered " << n << " hp.";

    this->curHP += n;
    if (this->curHP > this->maxHP) {
        ss << " (" << this->curHP - this->maxHP << " wasted)";
        this->curHP = this->maxHP;
    }

    GUI::AddMessage(ss.str());
}

void
Character::RecoverMP(int n) {
    std::stringstream ss;

    ss << "Recovered " << n << " mp.";

    this->curMP += n;
    if (this->curMP > this->maxMP) {
        ss << " (" << this->curMP - this->maxMP << " wasted)";
        this->curMP = this->maxMP;
    }

    GUI::AddMessage(ss.str());
}

void
Character::ReadBookOrScroll(int n) {
    Item *item;
    StatTome *stattome;
    std::list<Item*>::iterator it;

    if (n < 0)
        return;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::STAT_TOME) {
            if (n == 0)
                break;
            --n;
        }
    }

    if (n != 0) {
        GUI::Alert("Something went wrong trying to find the selected potion...");
        return;
    }

    this->Inventory.erase(it);
    //Now we deal with the effects of the potion
    stattome = (StatTome*) item;
    stattome->ApplyEffects(this);
    delete stattome;
}

void
Character::RandomCurse(int potency) {
    std::stringstream ss;
    int effect;

    ss << "CURSE! ";

    //Standard XP down curse
    effect = rand() % (potency * potency) + 1;
    this->XP -= effect;
    ss << "Amnesia -- You forget some of your recent adventures!";
    ss << " (" << effect << "XP lost)";
    GUI::Alert(ss.str());
}

Character::~Character(void) {
    Item *item;
    for (std::list<Item*>::iterator it = this->Inventory.begin();
    it != this->Inventory.end(); it++) {
        item = &**it;
        switch (item->type) {
            case Item::POTION:
                delete (Potion*) item;
                break;
            case Item::STAT_TOME:
                delete (StatTome*) item;
                break;
            case Item::TREASURE_T:
                delete (Treasure*) item;
                break;
            case Item::GENERIC:
            case Item::LAST_TYPE:
            default:
                delete item;
                break;
        }
    }
}
