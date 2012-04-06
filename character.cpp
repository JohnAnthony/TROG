#include "character.hpp"

Character::Character(Character::Race inRace, Character::Class inClass) {
    this->cclass = inClass;
    this->race = inRace;
    this->XP = 0;
    this->Level = 1;

    this->sight_range = 5;
    this->maxHP  = 30;
    this->maxMP  = 0;
    this->maxSTR = 20;
    this->maxTOU = 20;
    this->maxATT = 20;
    this->maxDEF = 20;

    if (inRace == Character::ELF) {
        this->sight_range += 2;
        this->maxATT += 2;
        this->maxDEF += 2;
        this->maxTOU -= 4;
    }
    else { // Human
    }

    this->FullyRestore();
}

void
Character::FullyRestore(void) {
    this->curHP = this->maxHP;
    this->curMP = this->maxMP;
    this->curSTR = this->maxSTR;
    this->curTOU = this->maxTOU;
    this->curATT = this->maxATT;
    this->curDEF = this->maxDEF;
}

void
Character::MoveTo(Point p) {
    this->pos = p;
}
