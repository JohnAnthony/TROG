#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include <string>

class Equippable : public Item {
    public:
    enum Category {
        SHORT_SWORD,
        LIGHT_SHIELD,
        LIGHT_CHAIN,
        HOLY_SYMBOL,
        STAFF,
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
