#include "spell.hpp"
#include "gui.hpp"
#include "character.hpp"
#include <sstream>

Spell::Spell(void (*infunc)(Game *g), const char *inName, int inMPCost,
int inXPCost, const char *inDescript) :
    name(inName),
    description(inDescript) {
    this->function = infunc;
    this->MPcost = inMPCost;
    this->XPcost = inXPCost;
}

//Folk
void Spell::Hearth_Heal(Game *g) {
    Spell::GenericHeal(g->character, 1);
    g->cur_level->GiveEnemiesTurns(g->character, 10);   
}

//Elf
void Spell::Minor_Sight(Game *g) {}
void Spell::Major_Sight(Game *g) {}
void Spell::Greater_Sight(Game *g) {}

//Divine
//
void Spell::Minor_Heal(Game *g) {
    Spell::GenericHeal(g->character, 1);
    g->cur_level->GiveEnemiesTurns(g->character, 1);
}

void Spell::Lesser_Heal(Game *g) {
    Spell::GenericHeal(g->character, 2);
    g->cur_level->GiveEnemiesTurns(g->character, 1);
}

void Spell::Light_Heal(Game *g) {
    Spell::GenericHeal(g->character, 3);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Moderate_Heal(Game *g) {
    Spell::GenericHeal(g->character, 4);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Average_Heal(Game *g) {
    Spell::GenericHeal(g->character, 5);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Strong_Heal(Game *g) {
    Spell::GenericHeal(g->character, 6);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Major_Heal(Game *g) {
    Spell::GenericHeal(g->character, 7);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Greater_Heal(Game *g) {
    Spell::GenericHeal(g->character, 8);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

void Spell::Turning_Touch(Game *g) {}
void Spell::Turn_Undead(Game *g) {}

//Arcane
void Spell::Fire_Touch(Game *g) {}
void Spell::Chill_Touch(Game *g) {}
void Spell::Fire_Bolt(Game *g) {}
void Spell::Ice_Bolt(Game *g) {}
void Spell::Fire_Burst(Game *g) {}
void Spell::Ice_Burst(Game *g) {}

void Spell::GenericHeal(Character *c, int potency) {
    std::stringstream ss;
    int effect;

    effect = 4;
    for (int i = 0; i < potency; ++i)
        effect += rand() % 8 + 1;

    c->Heal(effect);
}
