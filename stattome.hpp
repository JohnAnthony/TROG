#ifndef STATTOME_H
#define STATTOME_H

#include "item.hpp"

class StatTome : public Item {
    public:

    enum Category {
        HEALTH,
        ENERGY,
        STRENGTH,
        TOUGHNESS,
        SKILL,
        DODGE,
        MAGERY,
        WILL,
        XP,
        LAST_CATEGORY
    };

    StatTome(unsigned int pot);
    std::string GetName(void);
    Category category;

    unsigned int potency;
};

#endif
