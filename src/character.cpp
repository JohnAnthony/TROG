#include "character.hpp"
#include "gui.hpp"
#include "equippable.hpp"
#include <sstream>
#include <cstdlib>

typedef Equippable EQ;

static Affinity ClassAffinities[Character::LAST_CLASS] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV      SIGHT
    {10,    0,  8,  4,  8,  3,  0,  2,  0,      0},  //BARBARIAN
    {5,     8,  5,  3,  5,  3,  8,  5,  0,      0},  //CLERIC
    {5,     8,  5,  3,  5,  3,  10, 4,  0,      0},  //DRUID
    {8,     0,  8,  3,  9,  3,  0,  3,  0,      0},  //FIGHTER
    {8,     2,  8,  3,  8,  2,  4,  5,  0,      0},  //PALADIN
    {4,     8,  3,  2,  2,  2,  10, 4,  0,      0},  //SAGE
    {6,     0,  5,  3,  9,  5,  0,  2,  0,      0},  //THIEF
    {4,     10, 2,  2,  2,  2,  12, 4,  0,      0},  //WIZARD
};
static Affinity RaceAffinities[Character::LAST_RACE] {
    //HP    MP  STR TOU ATT DEF MAG WIL MV      SIGHT
    {10,    -99,2,  4,  2,  2,  -99,5,  200,    8},  //DWARF
    {4,     2,  2,  1,  4,  4,  0,  3, -100,    8},  //ELF
    {8,     0,  4,  3,  2,  3,  0,  1,  0,      7},  //GREY ORC
    {6,     0,  1,  1,  4,  5,  0,  4,  100,    6},  //HALFLING
    {7,     1,  2,  2,  3,  3,  0,  3,  -50,    6},  //HALF_ELF
    {8,     0,  3,  2,  3,  3,  0,  2,  0,      6},  //HALF_ORC
    {8,     0,  2,  2,  3,  3,  0,  3,  0,      5},  //HUMAN
    {9,     0,  3,  2,  3,  3,  0,  2,  0,      6},  //LIZARDFOLK
};

Character::Character(std::string inName, Character::Race inRace,
Character::Class inClass) {
    this->name = inName;
    this->cclass = inClass;
    this->race = inRace;
    this->XP = 0;
    this->Level = 1;
    this->symbol = '@';
    this->colour = COL_GREEN;
    this->affinity = this->SumAffinities(inRace, inClass);
    this->next_level = 1000;
    this->gold = 0;

    this->deepest_visited = 1;
    this->monsters_killed = 0;
    this->total_gold_collected = 0;
    this->potions_drunk = 0;
    this->books_read = 0;
    this->toughest_defeated = NULL;

    //Startign stats
    this->baseHP  = this->affinity.hp;
    this->baseMP  = this->affinity.mp;
    this->baseSTR = this->affinity.str;
    this->baseTOU = this->affinity.tou;
    this->baseATT = this->affinity.att;
    this->baseDEF = this->affinity.def;
    this->baseMAG = this->affinity.mag;
    this->baseWIL = this->affinity.wil;
    this->baseSIGHT = this->affinity.sight;
    this->baseMV = 1000 + this->affinity.mv;

    // Handle equipment for different classes
    this->equipment[HEAD] = NULL;
    this->equipment[BODY] = NULL;
    this->equipment[WEAPON] = NULL;
    this->equipment[SHIELD] = NULL;
    this->equipment[GLOVES] = NULL;
    this->equipment[BOOTS] = NULL;
    this->equipment[RING1] = NULL;
    this->equipment[RING2] = NULL;
    this->equipment[NECK] = NULL;

    switch(this->cclass) {
        case Character::BARBARIAN:
            if (rand() % 2)
                this->equipment[WEAPON] = new EQ(EQ::TWO_HANDED_AXE, 1);
            else
                this->equipment[WEAPON] = new EQ(EQ::TWO_HANDED_SWORD, 1);
            this->equipment[BODY] = new EQ(EQ::CHAINMAIL, 1);
            break;
        case Character::CLERIC:
            if (rand() % 2)
                this->equipment[WEAPON] = new EQ(EQ::MACE, 1);
            else
                this->equipment[WEAPON] = new EQ(EQ::FLAIL, 1);
            this->equipment[SHIELD] = new EQ(EQ::HEAVY_SHIELD, 1);
            this->equipment[BODY] = new EQ(EQ::CHAINMAIL, 1);
            break;
        case Character::DRUID:
            this->equipment[WEAPON] = new EQ(EQ::STAFF, 1);
            this->equipment[BODY] = new EQ(EQ::LEATHER_ARMOUR, 1);
            break;
        case Character::FIGHTER:
            this->equipment[WEAPON] = new EQ(EQ::LONGSWORD, 1);
            this->equipment[SHIELD] = new EQ(EQ::LIGHT_SHIELD, 1);
            this->equipment[BODY] = new EQ(EQ::CHAINMAIL, 1);
            break;
        case Character::PALADIN:
            this->equipment[WEAPON] = new EQ(EQ::LONGSWORD, 1);
            this->equipment[SHIELD] = new EQ(EQ::LIGHT_SHIELD, 1);
            this->equipment[BODY] = new EQ(EQ::CHAINMAIL, 1);
            this->equipment[NECK] = new EQ(EQ::HOLY_SYMBOL, 1);
            break;
        case Character::SAGE:
            this->equipment[WEAPON] = new EQ(EQ::STAFF, 1);
            this->equipment[BODY] = new EQ(EQ::ROBES, 1);
            break;
        case Character::THIEF:
            if (rand() % 2)
                this->equipment[WEAPON] = new EQ(EQ::SHORT_SWORD, 1);
            else
                this->equipment[WEAPON] = new EQ(EQ::DAGGER, 1);
            this->equipment[SHIELD] = new EQ(EQ::LIGHT_SHIELD, 1);
            this->equipment[BODY] = new EQ(EQ::LEATHER_ARMOUR, 1);
            break;
        case Character::WIZARD:
            this->equipment[WEAPON] = new EQ(EQ::STAFF, 1);
            this->equipment[BODY] = new EQ(EQ::ROBES, 1);
            break;
        case Character::LAST_CLASS:
        default:
            break;
    }

    //Races sometimes start with extra stuff. Humans get torches, elves get bows
    if (this->race == Character::HUMAN)
        this->ItemToInventory((Item*) new EQ(EQ::TORCH, 1));

    this->FullyRestore();
}

void
Character::FullyRestore(void) {
    this->curHP = this->baseHP;
    this->curMP = this->baseMP;

    this->RecalcEffective();
}

void
Character::LevelUp(void) {
    std::stringstream ss;
    int i;

    this->Level++;
    if (this->affinity.hp > 0) {
        i = rand() % this->affinity.hp + 1;
        this->baseHP += i;
        this->curHP += i;
    }
    if (this->affinity.mp > 0) {
        i = rand() % this->affinity.mp + 1;
        this->baseMP += i;
        this->curMP += i;
    }
    if (this->affinity.str > 0)
        this->baseSTR += rand() % this->affinity.str + 1;
    if (this->affinity.tou > 0)
        this->baseTOU += rand() % this->affinity.tou + 1;
    if (this->affinity.att > 0)
        this->baseATT += rand() % this->affinity.att + 1;
    if (this->affinity.def > 0)
        this->baseDEF += rand() % this->affinity.def + 1;
    if (this->affinity.mag > 0)
        this->baseMAG += rand() % this->affinity.mag + 1;
    if (this->affinity.wil > 0)
        this->baseWIL += rand() % this->affinity.wil + 1;
    // Note that mv and sight don't get modified with everythign else

    this->next_level += this->Level * 1000;
    this->RecalcEffective();

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
    if (this->race == Character::HUMAN)
        quant *= 1.10;
    else if (this->race == Character::HALF_ELF)
        quant *= 1.05;
    else if (this->race == Character::HALF_ORC)
        quant *= 1.05;

    this->XP += quant;
    if (this->XP >= this->next_level)
        this->LevelUp();
}

void
Character::ItemToInventory(Item *i) {
    Inventory.push_front(i);
}

void
Character::DrinkPotion(int n) {
    Item *item = NULL;
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

    if (n != 0 || !item) {
        GUI::Alert("Something went wrong trying to find the selected potion...");
        return;
    }

    this->Inventory.erase(it);
    //Now we deal with the effects of the potion
    potion = (Potion*) item;
    potion->ApplyEffects(this);

    this->potions_drunk ++;
    delete potion;
}

void
Character::Heal(int n) {
    std::stringstream ss;

    ss << "Recovered " << n << " hp.";

    this->curHP += n;
    if (this->curHP > this->baseHP) {
        ss << " (" << this->curHP - this->baseHP << " wasted)";
        this->curHP = this->baseHP;
    }

    GUI::AddMessage(ss.str());
}

void
Character::RecoverMP(int n) {
    std::stringstream ss;

    ss << "Recovered " << n << " mp.";

    this->curMP += n;
    if (this->curMP > this->baseMP) {
        ss << " (" << this->curMP - this->baseMP << " wasted)";
        this->curMP = this->baseMP;
    }

    GUI::AddMessage(ss.str());
}

void
Character::ReadBookOrScroll(int n) {
    Item *item = NULL;
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

    if (n != 0 || !item) {
        GUI::Alert("Something went wrong trying to find the selected potion.");
        return;
    }

    this->Inventory.erase(it);
    //Now we deal with the effects of the potion
    stattome = (StatTome*) item;
    stattome->ApplyEffects(this);

    this->books_read ++;
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

int
Character::getXP(void) {
    return this->XP;
}

bool
Character::isAlive(void) {
    return this->curHP > 0;
}

void
Character::RecalcEffective(void) {
    this->curSTR = this->baseSTR;
    this->curTOU = this->baseTOU;
    this->curATT = this->baseATT;
    this->curDEF = this->baseDEF;
    this->curMAG = this->baseMAG;
    this->curWIL = this->baseWIL;
    this->curSIGHT = this->baseSIGHT;
    this->curMV = this->baseMV;

    for (unsigned int i = 0; i < (int) LAST_EQUIP_LOCATION; ++i) {
        if (!this->equipment[i])
            continue;
        this->curSTR += this->equipment[i]->modSTR;
        this->curTOU += this->equipment[i]->modTOU;
        this->curATT += this->equipment[i]->modATT;
        this->curDEF += this->equipment[i]->modDEF;
        this->curMAG += this->equipment[i]->modMAG;
        this->curWIL += this->equipment[i]->modWIL;
        this->curSIGHT += this->equipment[i]->modSIGHT;
        this->curMV += this->equipment[i]->modMV;
    }

    this->curMV = MAX(this->curMV, 500);

    //Dwarves don't get any bonuses for using a torch
    if (this->equipment[SHIELD]) {
        if (this->equipment[SHIELD]->category == EQ::TORCH 
        && this->race == Character::DWARF) {
            this->curSIGHT -= 2;
        }
    }
}

std::string 
Character::getEquipmentName(EquipLocations el) {
    if (!this->equipment[el])
        return "NONE";
    return this->equipment[el]->GetName();
}

std::string 
Character::getEquipmentNameWithQuality(EquipLocations el) {
    if (!this->equipment[el])
        return "NONE";
    return this->equipment[el]->getNameWithQuality();
}

int 
Character::getEquipmentQuality(EquipLocations el) {
    if (!this->equipment[el])
        return 0;
    return this->equipment[el]->getQuality();
}

void
Character::TakeDamage(int n) {
    //Halfling luck
    if (this->race == Character::HALFLING && this->curHP - n <= 0) {
        if (rand() % 100 <= 50) {
            this->HandleHalflingLuck();
            return;
        }
    }

    this->curHP -= n;
}

void
Character::HandleHalflingLuck(void) {
    static char const * const msg[] = {
        "You dodge a blow that by rights should hvae killed you.",
        "What miraculous halfling luck!",
        NULL
    };

    GUI::Alert2(msg);
    GUI::AddMessage("Previous damage notification negated (halfling luck)");
}

void
Character::GrantLevel(void) {
    this->GiveXP(this->next_level - this->getXP());
}

void
Character::Unequip(EquipLocations loc) {
    if (!this->equipment[loc])
        return;

    this->ItemToInventory(this->equipment[loc]);
    this->equipment[loc] = NULL;

    this->RecalcEffective();
    return;
}

bool //Return true only if the new item is equipped
Character::Equip(EQ *e) {
    std::stringstream ss;

    if (e->location == SHIELD) {
        if (this->equipment[WEAPON]) {
            if (this->equipment[WEAPON]->isTwoHanded()) {
                GUI::Alert("You can't equip this with a two-handed weapon.");
                return false;
            }
        }
    }

    if (e->location == WEAPON && e->isTwoHanded()) {
        if (this->equipment[SHIELD]) {
            GUI::Alert("You must have two hands free to equipt a two-handed weapon");
            return false;
        }
    }

    if (!this->isEquipSlotFree(e->location)) {
        ss << "Unequip " << this->equipment[e->location]->getNameWithQuality();
        ss << " ?";

        if (! GUI::BinaryChoice(ss.str(), 'y', 'n') )
            return false;
        this->Unequip(e->location);
    }

    if (!this->isEquipSlotFree(e->location))
        return false;

    this->equipment[e->location] = e;
    this->RecalcEffective();
    return true;
}

bool
Character::isEquipSlotFree(EquipLocations loc) {
    return !this->equipment[loc];
}

Potion* 
Character::PotionFromInventory(int n) {
    std::list<Item*>::iterator it;
    Item* item;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); it++) {
        item = &**it;
        if (item->type != Item::POTION)
            continue;
        if (n == 0)
            return (Potion*) item;
        --n;
    }

    return NULL;
}

EQ* 
Character::EquippableFromInventory(int n) {
    std::list<Item*>::iterator it;
    Item* item;

    for (int i = 0; i < (int) LAST_EQUIP_LOCATION; ++i) {
        if (!this->equipment[i])
            continue;
        if (n == 0)
            return (EQ*) this->equipment[i];
        --n;
    }

    for (it = this->Inventory.begin(); it != this->Inventory.end(); it++) {
        item = &**it;
        if (item->type != Item::EQUIPPABLE)
            continue;
        if (n == 0)
            return (EQ*) item;
        --n;
    }

    return NULL;
}

StatTome* 
Character::StatTomeFromInventory(int n) {
    std::list<Item*>::iterator it;
    Item* item;

    for (it = this->Inventory.begin(); it != this->Inventory.end(); it++) {
        item = &**it;
        if (item->type != Item::STAT_TOME)
            continue;
        if (n == 0)
            return (StatTome*) item;
        --n;
    }

    return NULL;
}

Spell* 
Character::SpellFromList(int n) {
    std::list<Spell*>::iterator it;

    for (it = this->SpellList.begin(); it != this->SpellList.end(); it++) {
        if (n == 0)
            return &**it;
        --n;
    }

    return NULL;
}

void
Character::GiveGold(unsigned int n) {
    this->gold += n;
    this->total_gold_collected += n;
}

int
Character::getGold(void) {
    return this->gold;
}

void
Character::AddSpell(Spell* sp) {
    for (std::list<Spell*>::iterator it = this->SpellList.begin(); it !=
    this->SpellList.end(); it++) {
        if (&**it == sp)
            return;
    }
    this->SpellList.push_back(sp);
}

void
Character::GiveAllSpells(void) {
    for (unsigned int i = 0; i < LENGTH(FolkSpells); ++i)
        this->AddSpell(&FolkSpells[i]);
    for (unsigned int i = 0; i < LENGTH(DivineSpells); ++i)
        this->AddSpell(&DivineSpells[i]);
    for (unsigned int i = 0; i < LENGTH(ArcaneSpells); ++i)
        this->AddSpell(&ArcaneSpells[i]);
    for (unsigned int i = 0; i < LENGTH(ElfSpells); ++i)
        this->AddSpell(&ElfSpells[i]);
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
            case Item::EQUIPPABLE:
                delete (EQ*) item;
                break;
            case Item::GENERIC:
            case Item::LAST_TYPE:
            default:
                delete item;
                break;
        }
    }
    if (this->equipment[HEAD])
        delete this->equipment[HEAD];
    if (this->equipment[BODY])
        delete this->equipment[BODY];
    if (this->equipment[WEAPON])
        delete this->equipment[WEAPON];
    if (this->equipment[SHIELD])
        delete this->equipment[SHIELD];
    if (this->equipment[GLOVES])
        delete this->equipment[GLOVES];
    if (this->equipment[BOOTS])
        delete this->equipment[BOOTS];
    if (this->equipment[RING1])
        delete this->equipment[RING1];
    if (this->equipment[RING2])
        delete this->equipment[RING2];
    if (this->equipment[NECK])
        delete this->equipment[NECK];
}
