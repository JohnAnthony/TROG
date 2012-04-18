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
        LESSER,
        LIGHT,
        MODERATE,
        AVERAGE,
        STRONG,
        LAST_POTENCY
    };

    enum Category {
        HEALING,
        ENERVATION,
        REJUVENATION,
        LAST_CATEGORY
    };

    Potion(Potion::Potency inPot, Potion::Category inCat);
    ~Potion();
    void ApplyEffects(Character *c);
    std::string GetName(void);
    std::string GetLongDescription(void);

    Potency potency;
    Category category;
};

#endif
