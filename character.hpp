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
    int mag;
    int wil;
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

    Character(std::string inName, Character::Race inRace, Character::Class inClass);
    void FullyRestore(void);
    void LevelUp(void);
    void MoveTo(Point p);
    std::string ClassString(void);
    std::string RaceString(void);
    Affinity SumAffinities(Race inRace, Class inClass);

    int XP;
    int next_level;
    int Level;
    Class cclass;
    Race race;
    Affinity affinity;
    std::string name;
};


#endif
