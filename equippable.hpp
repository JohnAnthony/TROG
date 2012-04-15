#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include <string>

class Equippable : public Item {
    public:
    enum Category {
        DAGGER,
        SHORT_SWORD,
        LONGSWORD,
        MACE,
        FLAIL,
        STAFF,
        TWO_HANDED_SWORD,
        TWO_HANDED_AXE,
        LIGHT_SHIELD,
        HEAVY_SHIELD,
        ROBES,
        LEATHER_ARMOUR,
        CHAINMAIL,
        PLATEMAIL,
        HOLY_SYMBOL,
        LAST_CATEGORY
    };

    Equippable(Equippable::Category inCat, int pot);
    std::string getName(void);
    std::string getNameWithQuality(void);
    int getQuality(void);

    EquipLocations location;
    Equippable::Category category;
    int potency;
    int modHP;
    int modMP;
    int modSTR;
    int modTOU;
    int modATT;
    int modDEF;
    int modMAG;
    int modWIL;
};

#endif
