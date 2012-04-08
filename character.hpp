#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.hpp"
#include "geometry.hpp"
#include <string>

typedef struct {
    int hp;
    int mp;
    int str;
    int tou;
    int att;
    int def;
    int mv;
    int sight;
} Affinity;

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
        SAGE,
        LAST_CLASS
    } Class;
    typedef enum {
        HUMAN,
        ELF,
        DWARF,
        HALF_ELF,
        HALFLING,
        HALF_ORC,
        ORC,
        LIZARDFOLK,
        LAST_RACE
    } Race;

    Character(Character::Race inRace, Character::Class inClass);
    void FullyRestore(void);
    void MoveTo(Point p);
    std::string ClassString(void);
    std::string RaceString(void);
    Affinity SumAffinities(Race inRace, Class inClass);

    int XP;
    int Level;
    Class cclass;
    Race race;
    Affinity affinity;
};


#endif
