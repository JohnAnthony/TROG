#ifndef POTION_H
#define POTION_H

#include "item.hpp"
#include <string>

class Potion : public Item {
    public:
    enum Key {
        HEAL_WATERY,
        HEAL_MINOR,
        HEAL_LIGHT,
        HEAL_MODERATE,
        LAST_KEY
    };

    enum Category {
        HEALING,
        REJUVENATION
    };

    Potion(const char *inName, Potion::Key inKey, Potion::Category inCat, int inMin, int inMax);
    static Potion* GetPotion(Potion::Key k);

    Key key;
    Category category;
    int minPOTENCY;
    int maxPOTENCY;
};

static Potion PotionList[Potion::LAST_KEY] = {
Potion( "Watered-down healing potion",  Potion::HEAL_WATERY,    Potion::HEALING,    1,  4),
Potion( "Minor healing potion",         Potion::HEAL_MINOR,     Potion::HEALING,    2,  8),
Potion( "Light healing potion",         Potion::HEAL_LIGHT,     Potion::HEALING,    3,  12),
Potion( "Moderate healing potion",      Potion::HEAL_MODERATE,  Potion::HEALING,    4,  16),
};

#endif
