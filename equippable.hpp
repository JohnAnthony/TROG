#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"

class Equippable : public Item {
    public:
    enum Category {
        SWORD,
        LIGHT_SHIELD,
        LIGHT_CHAIN,
        HOLY_SYMBOL,
        STAFF,
        LAST_CATEGORY
    };
    enum Location {
        HEAD,
        BODY,
        R_HAND,
        L_HAND,
        GLOVES,
        FEET,
        FINGER,
        NECK,
        LAST_LOCATION
    };

    Equippable(Equippable::Category inCat, int pot);

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
