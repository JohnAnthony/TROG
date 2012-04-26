#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include <string>

/*
=================================================
TROG
Equippable ingame items
=================================================
*/
class Equippable : public Item
{
    public:
    enum Category
    {
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
        TORCH,
        LAST_CATEGORY
    };

    Equippable(Equippable::Category inCat, int pot);
    std::string GetName(void);
    std::string getNameWithQuality(void);
    int getQuality(void);
    static Equippable* NewRandomEquippable(int pot);
    void ShiningChance(void);
    bool isTwoHanded(void);
    char const * const GetLongDescription(void);

    EquipLocations location;
    Equippable::Category category;
    int potency;
    int modSTR;
    int modTOU;
    int modATT;
    int modDEF;
    int modMAG;
    int modWIL;
    int modSIGHT;
    int modMV;
};

#endif
