#include "character.hpp"

Character::Character(Character::Race inRace, Character::Class inClass) {
    this->cclass = inClass;
    this->race = inRace;
    this->XP = 0;
    this->Level = 1;
    this->symbol = '@';
    this->colour = CustColour::GREEN;

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

std::string
Character::ClassString(void) {
    switch (this->cclass) {
        case Character::FIGHTER:
            return "FIGHTER";
        case Character::THIEF:
            return "THIEF";
        case Character::WIZARD:
            return "WIZARD";
        case Character::CLERIC:
            return "CLERIC";
        case Character::BARBARIAN:
            return "BARBARIAN";
        case Character::PALADIN:
            return "PALADIN";
        case Character::DRUID:
            return "DRUID";
        case Character::SAGE:
            return "SAGE";
        default:
            return "UNKNOWN_CLASS";
    }
}

std::string
Character::RaceString(void) {
    switch (this->race) {
        case Character::HUMAN:
            return "HUMAN";
        case Character::ELF:
            return "ELF";
        case Character::DWARF:
            return "DWARF";
        case Character::HALF_ELF:
            return "HALF_ELF";
        case Character::HALFLING:
            return "HALFLING";
        case Character::HALF_ORC:
            return "HALF_ORC";
        case Character::ORC:
            return "ORC";
        case Character::LIZARDFOLK:
            return "LIZARDFOLK";
        default:
            return "UNKNOWN_RACE";
    }
}
