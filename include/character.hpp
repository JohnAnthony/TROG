#ifndef CHARACTER_H
#define CHARACTER_H

#include "geometry.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "potion.hpp"
#include "stattome.hpp"
#include "equippable.hpp"
#include <string>
#include <list>

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
class Potion;
class StatTome;

class Character {
    public:
    typedef enum {
        BARBARIAN,
        CLERIC,
        DRUID,
        FIGHTER,
        PALADIN,
        SAGE,
        THIEF,
        WIZARD,
        LAST_CLASS
    } Class;
    typedef enum {
        DWARF,
        ELF,
        GREY_ORC,
        HALFLING,
        HALF_ELF,
        HALF_ORC,
        HUMAN,
        LIZARDFOLK,
        LAST_RACE
    } Race;

    Character(std::string inName, Character::Race inRace,
        Character::Class inClass);
    ~Character(void);
    void FullyRestore(void);
    void LevelUp(void);
    void MoveTo(Point p);
    std::string GetClassString(void);
    std::string GetRaceString(void);
    static std::string ClassAsString(Character::Class inClass);
    static std::string RaceAsString(Character::Race inRace);
    Affinity SumAffinities(Race inRace, Class inClass);
    void GiveXP(unsigned int quant);
    void Attack(Enemy *e);
    void ItemToInventory(Item *i);
    void DrinkPotion(int n);
    void Heal(int n);
    void RecoverMP(int n);
    void ReadBookOrScroll(int n);
    void RandomCurse(int potency);
    bool isAlive(void);
    void RecalcEffective(void);
    std::string getEquipmentName(EquipLocations el);
    std::string getEquipmentNameWithQuality(EquipLocations el);
    int getEquipmentQuality(EquipLocations el);
    void TakeDamage(int n);
    void HandleHalflingLuck(void);
    void GrantLevel(void);
    void Unequip(EquipLocations loc);
    bool Equip(Equippable *e);
    bool isEquipSlotFree(EquipLocations loc);
    Potion* PotionFromInventory(int n);
    Equippable* EquippableFromInventory(int n);
    StatTome* StatTomeFromInventory(int n);

    //Getters(!)
    int getXP(void);
    int getCurHP(void);

    char symbol;
    Colour colour;
    Point pos;
    int next_level;
    int Level;
    Class cclass;
    Race race;
    Affinity affinity;
    std::string name;
    std::list<Item*> Inventory;
    Equippable *equipment[LAST_EQUIP_LOCATION];
    int gold;

    int baseHP, curHP;
    int baseMP, curMP;
    int baseSTR, curSTR;
    int baseTOU, curTOU;
    int baseATT, curATT;
    int baseDEF, curDEF;
    int baseMAG, curMAG;
    int baseWIL, curWIL;
    int baseMV, curMV;
    int baseSIGHT, curSIGHT;

    unsigned int deepest_visited;

    private:
    int XP;
};

#endif
