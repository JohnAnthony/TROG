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

#endif
