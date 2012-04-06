#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.hpp"
#include "geometry.hpp"
#include <string>

class Character : public Entity {
    public:
    typedef enum {
        FIGHTER,
        THIEF,
        WIZARD,
        CLERIC,
        BARBARIAN,
        PALADIN,
        DRUID,
        SAGE
    } Class;
    typedef enum {
        HUMAN,
        ELF,
        DWARF,
        HALF_ELF,
        HALFLING,
        HALF_ORC,
        ORC,
        LIZARDFOLK
    } Race;

    Character(Character::Race inRace, Character::Class inClass);
    void FullyRestore(void);
    void MoveTo(Point p);
    std::string ClassString(void);
    std::string RaceString(void);
    int XP;
    int Level;
    Class cclass;
    Race race;
};

#endif
