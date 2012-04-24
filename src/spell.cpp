#include "spell.hpp"

Spell::Spell(void (*infunc)(Game *g, Enemy *target), int inCost,
const char *inDescript) : description(inDescript) {
    this->function = infunc;
    this->cost = inCost;
}

void
Spell::Hearth_Heal(Game *g, Enemy *target) {

}
