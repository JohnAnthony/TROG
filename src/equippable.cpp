#include "equippable.hpp"
#include "gui.hpp"
#include "geometry.hpp"
#include <sstream>

typedef struct {
    char const * const name;
    char const * const description;
} EquippableInfo;

static EquippableInfo EQUIPPABLE_INFO[Equippable::LAST_CATEGORY] = {
    {"Dagger", "A short knife with a wicked, jagged blade"},
    {"Short Sword", "A short stabbing blade"},
    {"Longsword", "A long, straight sword with a weighted pommel"},
    {"Mace", "A metal shaft with a spiked steel ball on the end"},
    {"Flail", "A metal shaft with a spiked steel ball hanging from a chain"},
    {"Staff", "A length of gnarled wood with a knob on the end"},
    {"Greatsword", "A huge sword suitable for use with two hands"},
    {"Greataxe", "A huge axe suitable for use with two hands"},
    {"Light Shield", "A two-foot disc of wood covered in felt"},
    {"Heavy Shield", "A three-foot disc of wood covered in steel rivets"},
    {"Robes", "Long, elaborate flowing garments"},
    {"Leather Armour", "Tanned animal hides crafted into armous"},
    {"Chainmail", "A full set of linked chain armour"},
    {"Platemail", "A full set of head-to-toe heavy plates. Very strong but cumbersome"},
    {"Holy Symbol", "A symbol of the goddess Aerwin attached to a fine, light chain"},
    {"Torch", "A 3-foot shaft of wood. The end is wrapped in flammable cloth and oil"},
    //LAST_CATEGORY
};

static char const * const ShiningAdjectives[] = {
    "Glowing",
    "Shining",
    "Radiant",
    "Gleaming",
    "Brilliant"
};

Equippable::Equippable(Equippable::Category inCat, int pot) {
    this->potency = pot;
    this->category = inCat;
    this->modSTR = 0;
    this->modTOU = 0;
    this->modATT = 0;
    this->modDEF = 0;
    this->modMAG = 0;
    this->modWIL = 0;
    this->modSIGHT = 0;
    this->modMV = 0;
    this->colour = COL_CYAN;
    this->type = Item::EQUIPPABLE;

    switch (inCat) {
        case Equippable::DAGGER:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 4;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->ShiningChance();
            this->location = WEAPON;
            break;
        case Equippable::SHORT_SWORD:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 2;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->ShiningChance();
            this->location = WEAPON;
            break;
        case Equippable::LONGSWORD:
            this->modSTR = 5 + DICEROLL(pot, 10);
            this->modATT = (5 + DICEROLL(pot, 10)) / 2;
            this->ShiningChance();
            this->location = WEAPON;
            break;
        case Equippable::MACE:
        case Equippable::FLAIL:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 2;
            this->modATT = (5 + DICEROLL(pot, 10)) / 2;
            this->ShiningChance();
            this->location = WEAPON;
            break;
        case Equippable::STAFF:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 4;
            this->modATT = (5 + DICEROLL(pot, 10)) / 4;
            this->modMAG = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::TWO_HANDED_SWORD:
        case Equippable::TWO_HANDED_AXE:
            this->modSTR = (5 + DICEROLL(pot, 10)) * 1.5;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->ShiningChance();
            this->location = WEAPON;
            break;
        case Equippable::LIGHT_SHIELD:
            this->modDEF = 5 + DICEROLL(pot, 10);
            this->ShiningChance();
            this->location = SHIELD;
            break;
        case Equippable::HEAVY_SHIELD:
            this->modDEF = 5 + DICEROLL(pot, 10) / 2;
            this->modTOU = 5 + DICEROLL(pot, 10) / 2;
            this->ShiningChance();
            this->location = SHIELD;
            break;
        case Equippable::ROBES:
            this->modMAG = 5 + DICEROLL(pot, 10) / 2;
            this->modWIL = 5 + DICEROLL(pot, 10) / 2;
            this->location = BODY;
            break;
        case Equippable::LEATHER_ARMOUR:
            this->modTOU = 5 + DICEROLL(pot, 10) / 2;
            this->location = BODY;
            this->location = BODY;
            break;
        case Equippable::CHAINMAIL:
            this->modTOU = 5 + DICEROLL(pot, 10);
            this->location = BODY;
            break;
        case Equippable::PLATEMAIL:
            this->modTOU = 5 + DICEROLL(pot, 10) * 1.5;
            this->modDEF = - (5 + DICEROLL(pot, 10) / 2);
            this->location = BODY;
            break;
        case Equippable::HOLY_SYMBOL:
            this->modWIL = 5 + DICEROLL(pot, 10);
            this->location = NECK;
            break;
        case Equippable::TORCH:
            this->modSIGHT = 3;
            this->location = SHIELD;
            break;
        case Equippable::LAST_CATEGORY:
        default:
            GUI::Alert("Error generating item!");
            break;
    }

    switch (this->location) {
        case HEAD:
            this->symbol = 'h';
            break;
        case BODY:
            this->symbol = 'a';
            break;
        case WEAPON:
            this->symbol = 'w';
            break;
        case SHIELD:
            this->symbol = 's';
            break;
        case GLOVES:
            this->symbol = 'g';
            break;
        case BOOTS:
            this->symbol = 'b';
            break;
        case RING1:
        case RING2:
            this->symbol = 'r';
            break;
        case NECK:
            this->symbol = 'n';
            break;
        case LAST_EQUIP_LOCATION:
        default:
            this->symbol = 'i';
            break;
    }

    //Special case symbols
    if (this->category == Equippable::TORCH) {
        this->symbol = 't';
        this->colour = COL_YELLOW;
    }
}

std::string 
Equippable::GetName(void) {
    std::stringstream ss;

    //Special qualifiers
    if (this->modSIGHT != 0)
        if (this->category != Equippable::TORCH)
            ss << ShiningAdjectives[this->modSIGHT - 1] << ' ';

    ss << EQUIPPABLE_INFO[this->category].name;
    return ss.str();
}

std::string
Equippable::getNameWithQuality(void) {
    std::stringstream ss;
    ss << this->GetName();
    ss << " (q:" << this->getQuality() << ")";
    return ss.str();
}

int
Equippable::getQuality(void) {
    int ret;

    //Special cases
    if (this->category == TORCH)
        return 0;

    //Procedurally generated cases
    ret = 0;
    ret += this->modSTR;
    ret += this->modTOU;
    ret += this->modATT;
    ret += this->modDEF;
    ret += this->modMAG;
    ret += this->modWIL;
    ret += this->modSIGHT * 10;
    ret -= this->modMV;

    return ret;
}

Equippable*
Equippable::NewRandomEquippable(int pot) {
    return new Equippable((Equippable::Category)(rand() % 
      Equippable::LAST_CATEGORY), pot);
}

void
Equippable::ShiningChance(void) {
    int maxBONUS;
    if (rand() % 100 < 1 && this->modSIGHT <= 5) {
        if (this->potency < 10)
            maxBONUS = 2;
        else if (this->potency < 20)
            maxBONUS = 3;
        else if (this->potency < 30)
            maxBONUS = 4;
        else
            maxBONUS = 5;
        this->modSIGHT = rand() % maxBONUS + 1;
    }
}

bool
Equippable::isTwoHanded(void) {
    if (this->category == Equippable::TWO_HANDED_AXE)
        return true;
    if (this->category == Equippable::TWO_HANDED_SWORD)
        return true;
    if (this->category == Equippable::STAFF)
        return true;
    return false;
}

char const * const
Equippable::GetLongDescription(void) {
    return EQUIPPABLE_INFO[this->category].description;
}
