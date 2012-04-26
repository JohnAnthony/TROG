#include "spell.hpp"
#include "gui.hpp"
#include "character.hpp"
#include <sstream>

Spell::Spell(const void (*infunc)(Game *g), const char *inName, int inMPCost,
int inXPCost, const char *inDescript) :
    function(infunc),
    MPcost(inMPCost),
    XPcost(inXPCost),
    name(inName),
    description(inDescript) {
}

//Folk
const void
Spell::Hearth_Heal(Game *g) {
    Spell::GenericHeal(g->character, 1);
    g->cur_level->GiveEnemiesTurns(g->character, 10);   
}

//Elf
const void
Spell::Minor_Sight(Game *g) {}
const void
Spell::Major_Sight(Game *g) {}
const void
Spell::Greater_Sight(Game *g) {}

//Divine
//
const void
Spell::Minor_Heal(Game *g) {
    Spell::GenericHeal(g->character, 1);
    g->cur_level->GiveEnemiesTurns(g->character, 1);
}

const void
Spell::Lesser_Heal(Game *g) {
    Spell::GenericHeal(g->character, 2);
    g->cur_level->GiveEnemiesTurns(g->character, 1);
}

const void
Spell::Light_Heal(Game *g) {
    Spell::GenericHeal(g->character, 3);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Moderate_Heal(Game *g) {
    Spell::GenericHeal(g->character, 4);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Average_Heal(Game *g) {
    Spell::GenericHeal(g->character, 5);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Strong_Heal(Game *g) {
    Spell::GenericHeal(g->character, 6);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Major_Heal(Game *g) {
    Spell::GenericHeal(g->character, 7);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Greater_Heal(Game *g) {
    Spell::GenericHeal(g->character, 8);
    g->cur_level->GiveEnemiesTurns(g->character, 1);   
}

const void
Spell::Turning_Touch(Game *g) {
    Direction::Type d;
    Point p;
    Enemy *target;

    d = GUI::GetDirection();
    p = GetRelativePoint(d, g->character->pos);
    target = g->cur_level->GetEnemy(p);

    if (!target) {
        GUI::AddMessage("Your spell finds no target");
        return;
    }

    if (target->parent_type->category != EnemyType::UNDEAD) {
        GUI::AddMessage("Your spell does not affect this type of creature.");
        return;
    }

    g->DoMagicAttack(g->character, target);
}

const void
Spell::Turn_Undead(Game *g) {}

//Arcane
const void
Spell::Fire_Touch(Game *g) {
    Direction::Type d;
    Point p;
    Enemy *target;

    d = GUI::GetDirection();
    p = GetRelativePoint(d, g->character->pos);
    target = g->cur_level->GetEnemy(p);

    if (!target) {
        GUI::AddMessage("Your spell finds no target");
        return;
    }

    g->DoMagicAttack(g->character, target);
}

const void
Spell::Chill_Touch(Game *g) {
    Direction::Type d;
    Point p;
    Enemy *target;

    d = GUI::GetDirection();
    p = GetRelativePoint(d, g->character->pos);
    target = g->cur_level->GetEnemy(p);

    if (!target) {
        GUI::AddMessage("Your spell finds no target");
        return;
    }

    g->DoMagicAttack(g->character, target);
}

const void
Spell::Fire_Bolt(Game *g) {
}

const void
Spell::Ice_Bolt(Game *g) {
}

const void
Spell::Fire_Burst(Game *g) {
}

const void
Spell::Ice_Burst(Game *g) {
}

void
Spell::GenericHeal(Character *c, int potency) {
    std::stringstream ss;
    int effect;

    effect = 4;
    for (int i = 0; i < potency; ++i)
        effect += rand() % 8 + 1;

    c->Heal(effect);
}
