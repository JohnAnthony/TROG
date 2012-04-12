#ifndef POTION_H
#define POTION_H

#include "character.hpp"
#include "item.hpp"
#include <string>

class Character;

class Potion : public Item {
    public:
    enum Strength {
        WATERY,
        MINOR,
        LIGHT,
        MODERATE,
        LAST_STRENGTH
    };

    enum Category {
        HEALING,
        ENERVATION,
        REJUVENATION,
        MISC,
        LAST_CATEGORY
    };

    Potion(Potion::Strength inStr, Potion::Category inCat);
    void ApplyEffects(Character *c);

    Strength strength;
    Category category;
};

// static Potion PotionList[Potion::LAST_KEY] = {
// Potion( "Watered-down healing potion",  Potion::HEAL_WATERY,    Potion::HEALING,    1,  4),
// Potion( "Minor healing potion",         Potion::HEAL_MINOR,     Potion::HEALING,    2,  8),
// Potion( "Light healing potion",         Potion::HEAL_LIGHT,     Potion::HEALING,    3,  12),
// Potion( "Moderate healing potion",      Potion::HEAL_MODERATE,  Potion::HEALING,    4,  16),
// };

#endif
