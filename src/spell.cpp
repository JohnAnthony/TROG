#include "spell.hpp"

Spell::Spell(void (*infunc)(Game *g, Enemy *target), int inMPCost, int inXPCost,
const char *inDescript) : description(inDescript) {
    this->function = infunc;
    this->MPcost = inMPCost;
    this->XPcost = inXPCost;
}

//Folk
void Spell::Hearth_Heal(Game *g, Enemy *target) {
    
}

//Elf
void Spell::Minor_Sight(Game *g, Enemy *target) {}
void Spell::Major_Sight(Game *g, Enemy *target) {}
void Spell::Greater_Sight(Game *g, Enemy *target) {}

//Divine
void Spell::Minor_Heal(Game *g, Enemy *target) {}
void Spell::Lesser_Heal(Game *g, Enemy *target) {}
void Spell::Light_Heal(Game *g, Enemy *target) {}
void Spell::Moderate_Heal(Game *g, Enemy *target) {}
void Spell::Average_Heal(Game *g, Enemy *target) {}
void Spell::Strong_Heal(Game *g, Enemy *target) {}
void Spell::Major_Heal(Game *g, Enemy *target) {}
void Spell::Greater_Heal(Game *g, Enemy *target) {}
void Spell::Turning_Touch(Game *g, Enemy *target) {}
void Spell::Turn_Undead(Game *g, Enemy *target) {}

//Arcane
void Spell::Fire_Bolt(Game *g, Enemy *target) {}
void Spell::Ice_Bolt(Game *g, Enemy *target) {}
void Spell::Fire_Burst(Game *g, Enemy *target) {}
void Spell::Ice_Burst(Game *g, Enemy *target) {}
