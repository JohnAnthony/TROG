#ifndef GAME_H
#define GAME_H

#include "level.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include "enemy.hpp"
#include "scrollable_menu.hpp"
#include <string>

class Level;

namespace GameMode {
    typedef enum {
        MAP_WALK,
        MAP_LOOK,
        INFO_SCREEN,
        CHARACTER_SCREEN,
        INVENTORY_SCREEN,
        POTION_SELECT,
        READING_SELECT,
        GEAR_SELECT,
        SPELL_SELECT,
        CHARACTER_RENAME_ONCE,
        LAST_MODE
    } Type;
}

class Game {
    public:
    Game(Character *c);
    ~Game(void);
    bool Run(void);
    GameMode::Type HandleInput(int c);
    void GoUpALevel(void);
    void GoDownALevel(void);
    void SwitchGameMode(GameMode::Type gmt);
    void MoveCharacter(Direction::Type d);
    void DrawLookTarget(void);
    void MoveLookTarget(Direction::Type d);
    void DoPickup(void);
    void HandleResize(int signal);
    void DoAttack(Character *c, Enemy *e);
    void DoWait(void);
    void RepopulatePotionMenu(void);
    void RepopulateBookMenu(void);
    void RepopulateEquipMenu(void);
    void RepopulateSpellMenu(void);
    void HandleEquipSelection(int n);
    void ReportStandingOn(void);

    Level*              levels;
    Level*              cur_level;
    Character*          character;
    int                 cur_floor;
    bool                running;
    bool                need_hard_redraw;
    GameMode::Type      game_mode;
    Point               target; //Look target
    ScrollableMenu      *PotionSelectMenu;
    ScrollableMenu      *BookSelectMenu;
    ScrollableMenu      *EquipSelectMenu;
    ScrollableMenu      *SpellSelectMenu;
};

#endif
