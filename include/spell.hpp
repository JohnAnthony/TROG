#ifndef SPELL_H
#define SPELL_H

#include "game.hpp"

class Spell {
    public:
    Spell(void (*infunc)(Game *g, Enemy *target), int inMPCost, int inXPCost,
        const char *inDescript);

    void (*function)(Game *g, Enemy *target);
    unsigned int MPcost;
    unsigned int XPcost;
    char const * const description;

    //Spell-specific functions
    //Folk
    static void Hearth_Heal(Game *g, Enemy *target);
    //Elf
    static void Minor_Sight(Game *g, Enemy *target);
    static void Major_Sight(Game *g, Enemy *target);
    static void Greater_Sight(Game *g, Enemy *target);
    //Divine
    static void Minor_Heal(Game *g, Enemy *target);
    static void Lesser_Heal(Game *g, Enemy *target);
    static void Light_Heal(Game *g, Enemy *target);
    static void Moderate_Heal(Game *g, Enemy *target);
    static void Average_Heal(Game *g, Enemy *target);
    static void Strong_Heal(Game *g, Enemy *target);
    static void Major_Heal(Game *g, Enemy *target);
    static void Greater_Heal(Game *g, Enemy *target);
    static void Turning_Touch(Game *g, Enemy *target);
    static void Turn_Undead(Game *g, Enemy *target);
    //Arcane
    static void Fire_Bolt(Game *g, Enemy *target);
    static void Ice_Bolt(Game *g, Enemy *target);
    static void Fire_Burst(Game *g, Enemy *target);
    static void Ice_Burst(Game *g, Enemy *target);
};

static Spell FolkSpells[] = {
//    Function              MPCost  XPCost  Description
Spell(Spell::Hearth_Heal,   4,      200,    "A slow but cheap heal. Takes 10 turns")
};

static Spell ElfSpells[] = {
//    Function              MPCost  XPCost  Description
Spell(Spell::Minor_Sight,   4,      500,    "Reveals a small local area"),
Spell(Spell::Major_Sight,   20,     2000,   "Reveals a large local area"),
Spell(Spell::Greater_Sight, 80,     6000,   "Reveals a complete level")
};

static Spell DivineSpells[] = {
//    Function              MPCost  XPCost  Description
Spell(Spell::Minor_Heal,    4,      200,    "Minor instant healing"),
Spell(Spell::Lesser_Heal,   8,      500,    "Lesser instant healing"),
Spell(Spell::Light_Heal,    15,     1500,   "Light instant healing"),
Spell(Spell::Moderate_Heal, 30,     3000,   "Moderate instant healing"),
Spell(Spell::Average_Heal,  50,     5000,   "Average instant healing"),
Spell(Spell::Strong_Heal,   80,     8000,   "Strong instant healing"),
Spell(Spell::Major_Heal,    100,    10000,  "Major instant healing"),
Spell(Spell::Greater_Heal,  150,    15000,  "Greater instant healing"),
Spell(Spell::Turning_Touch, 150,    180,    "Turns a single touched undead"),
Spell(Spell::Turn_Undead,   10,     1000,   "Turns all undead within 5 squares")
};

static Spell ArcaneSpells[] = {
Spell(Spell::Fire_Bolt,     4,      100,   "A singular magic blast fo fire"),
Spell(Spell::Ice_Bolt,      4,      100,   "A singular icy projectile"),
Spell(Spell::Fire_Burst,    20,     2000,  "A burst of fire engulfs all within 5 squares"),
Spell(Spell::Ice_Burst,     20,     2000,  "A burst of razor-sharp ice shards engulfs all within 5 squares")
};

#endif
