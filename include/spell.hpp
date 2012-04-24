#ifndef SPELL_H
#define SPELL_H

#include "game.hpp"

class Spell {
    public:
    Spell(void (*infunc)(Game *g, Enemy *target), int inCost,
        const char *inDescript);

    void (*function)(Game *g, Enemy *target);
    unsigned int cost;
    char const * const description;

    //Spell-specific functions
    static void Hearth_Heal(Game *g, Enemy *target);
};

static Spell FolkSpells[] = {

    //Function              Cost    Description
Spell(Spell::Hearth_Heal,   4,      "A slow but cheap heal. Takes 10 turns")

};

#endif
