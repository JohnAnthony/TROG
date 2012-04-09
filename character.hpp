#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.hpp"
#include "geometry.hpp"
#include "item.hpp"
#include "enemy.hpp"
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

class Enemy;

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
    std::string GetClassString(void);
    std::string GetRaceString(void);
    std::string ClassAsString(Character::Class inClass);
    std::string RaceAsString(Character::Race inRace);
    Affinity SumAffinities(Race inRace, Class inClass);
    void GiveXP(unsigned int quant);
    void Attack(Enemy *e);

    int XP;
    int next_level;
    int Level;

    Class cclass;
    Race race;
    Affinity affinity;
    std::string name;
    Item *helm;
    Item *armour;
    Item *weapon;
    Item *gloves;
    Item *boots;
    Item *ring1;
    Item *ring2;
    Item *neck;
};

#endif
