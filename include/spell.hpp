#ifndef SPELL_H
#define SPELL_H

#include "game.hpp"

class Game;

class Spell {
    public:
    Spell(const void (*infunc)(Game *g), const char *inName, int inMPCost,
        int inXPCost, const char *inDescript);

    const void (*function)(Game *g);
    const unsigned int MPcost;
    const unsigned int XPcost;
    char const * const name;
    char const * const description;

    //Spell-specific functions
    //Folk
    static const void Hearth_Heal(Game *g);
    //Elf
    static const void Minor_Sight(Game *g);
    static const void Major_Sight(Game *g);
    static const void Greater_Sight(Game *g);
    //Divine
    static const void Minor_Heal(Game *g);
    static const void Lesser_Heal(Game *g);
    static const void Light_Heal(Game *g);
    static const void Moderate_Heal(Game *g);
    static const void Average_Heal(Game *g);
    static const void Strong_Heal(Game *g);
    static const void Major_Heal(Game *g);
    static const void Greater_Heal(Game *g);
    static const void Turning_Touch(Game *g);
    static const void Turn_Undead(Game *g);
    //Arcane
    static const void Fire_Touch(Game *g);
    static const void Chill_Touch(Game *g);
    static const void Fire_Bolt(Game *g);
    static const void Ice_Bolt(Game *g);
    static const void Fire_Burst(Game *g);
    static const void Ice_Burst(Game *g);

    //Helper generic spells
    static void GenericHeal(Character *c, int potency);
    static void GenericTouch(Game *g);
};

static Spell FolkSpells[] = {
//    Function              Name                MPCost  XPCost  Description
Spell(Spell::Hearth_Heal,   "Hearth Heal",      4,      200,    "A slow but cheap heal. Takes 10 turns")
};

static Spell ElfSpells[] = {
//    Function              Name                MPCost  XPCost  Description
Spell(Spell::Minor_Sight,   "Minor Sight",      4,      500,    "Reveals a small local area"),
Spell(Spell::Major_Sight,   "Major Sight",      20,     2000,   "Reveals a large local area"),
Spell(Spell::Greater_Sight, "Greater Sight",    80,     6000,   "Reveals a complete level")
};

static Spell DivineSpells[] = {
//    Function              Name                MPCost  XPCost  Description
Spell(Spell::Minor_Heal,    "Minor Heal",       4,      200,    "Minor instant healing"),
Spell(Spell::Lesser_Heal,   "Lesser Heal",      8,      500,    "Lesser instant healing"),
Spell(Spell::Light_Heal,    "Light Heal",       15,     1500,   "Light instant healing"),
Spell(Spell::Moderate_Heal, "Moderate Heal",    30,     3000,   "Moderate instant healing"),
Spell(Spell::Average_Heal,  "Average Heal",     50,     5000,   "Average instant healing"),
Spell(Spell::Strong_Heal,   "Strong Heal",      80,     8000,   "Strong instant healing"),
Spell(Spell::Major_Heal,    "Major Heal",       100,    10000,  "Major instant healing"),
Spell(Spell::Greater_Heal,  "Greater Heal",     150,    15000,  "Greater instant healing"),
Spell(Spell::Turning_Touch, "Turning Touch",    2,      180,    "Turns a single touched undead"),
Spell(Spell::Turn_Undead,   "Turn Undead",      10,     1000,   "Turns all undead within 5 squares")
};

static Spell ArcaneSpells[] = {
//    Function              Name                MPCost  XPCost  Description
Spell(Spell::Fire_Touch,    "Fire Touch",       2,      40,    "A touch attack that chars the flesh"),
Spell(Spell::Chill_Touch,   "Chill Touch",      2,      40,    "A touch attack that chills the flesh"),
Spell(Spell::Fire_Bolt,     "Fire Bolt",        4,      400,   "A singular magic blast fo fire"),
Spell(Spell::Ice_Bolt,      "Ice Bolt",         4,      400,   "A singular icy projectile"),
Spell(Spell::Fire_Burst,    "Fire Burst",       20,     2000,  "A burst of fire engulfs all within 5 squares"),
Spell(Spell::Ice_Burst,     "Ice Burst",        20,     2000,  "A burst of razor-sharp ice shards engulfs all within 5 squares")
};

#endif
