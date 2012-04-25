#include "spell.hpp"

Spell::Spell(void (*infunc)(Game *g), const char *inName, int inMPCost,
int inXPCost, const char *inDescript) :
    description(inDescript) {
    this->name = (char*)"Derp";
    this->function = infunc;
    this->MPcost = inMPCost;
    this->XPcost = inXPCost;
}

//Folk
void Spell::Hearth_Heal(Game *g) {
    
}

//Elf
void Spell::Minor_Sight(Game *g) {}
void Spell::Major_Sight(Game *g) {}
void Spell::Greater_Sight(Game *g) {}

//Divine
void Spell::Minor_Heal(Game *g) {}
void Spell::Lesser_Heal(Game *g) {}
void Spell::Light_Heal(Game *g) {}
void Spell::Moderate_Heal(Game *g) {}
void Spell::Average_Heal(Game *g) {}
void Spell::Strong_Heal(Game *g) {}
void Spell::Major_Heal(Game *g) {}
void Spell::Greater_Heal(Game *g) {}
void Spell::Turning_Touch(Game *g) {}
void Spell::Turn_Undead(Game *g) {}

//Arcane
void Spell::Fire_Touch(Game *g) {}
void Spell::Chill_Touch(Game *g) {}
void Spell::Fire_Bolt(Game *g) {}
void Spell::Ice_Bolt(Game *g) {}
void Spell::Fire_Burst(Game *g) {}
void Spell::Ice_Burst(Game *g) {}
