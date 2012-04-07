#ifndef GAME_H
#define GAME_H

#include "level.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include <string>

class Level;

namespace GameMode {
    typedef enum {
        MAP_WALK,
        MAP_LOOK,
        INFO_SCREEN,
        CHARACTER_SCREEN,
        INVENTORY_SCREEN
    } Type;
}

class Game {
    public:
    Game(void);
    ~Game(void);
    void Run(void);
    void ShowMapInfo(void);
    void HandleInput(int c);
    void GoUpALevel(void);
    void GoDownALevel(void);
    void SwitchGameMode(GameMode::Type gmt);
    void MoveCamera(Direction::Type d);
    void DoRedraw(void);
    void MoveCharacter(Direction::Type d);
    void MakeStatusLine(void);
    void DrawLookTarget(void);
    void MoveLookTarget(Direction::Type d);
    void DrawAsOverlay(Point p, char c, int col);
    void RedrawStatus(void);

    int                 gold;
    Level*              levels;
    Level*              cur_level;
    Character*          character;
    int                 cur_floor;
    bool                running;
    GameMode::Type      game_mode;
    std::string         status_line;
    Point               target; //For examining things
};

#endif
