#ifndef POTION_H
#define POTION_H

#include "character.hpp"
#include "item.hpp"
#include <string>

class Character;

class Potion : public Item {
    public:
    enum Potency {
        MINOR,
        LIGHT,
        MODERATE,
        AVERAGE,
        LAST_POTENCY
    };

    enum Category {
        HEALING,
        ENERVATION,
        REJUVENATION,
        MISC,
        LAST_CATEGORY
    };

    Potion(Potion::Potency inPot, Potion::Category inCat);
    void ApplyEffects(Character *c);
    std::string GetName(void);

    Potency potency;
    Category category;
};

#endif
