#ifndef STATTOME_H
#define STATTOME_H

#include "item.hpp"

class StatTome : public Item {
    public:
    StatTome(unsigned int pot);
    std::string GetName(void);

    unsigned int potency;
};

#endif
