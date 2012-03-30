#ifndef GAME_H
#define GAHE_H

#include "level.hpp"
#include "character.hpp"
#include "geometry.hpp"

namespace GameMode {
    typedef enum {
        MAP_WALK,
        MAP_INFO,
        CHARACTER_SCREEN
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

    Level*              levels;
    Level*              cur_level;
    Character*          character;
    int                 cur_floor;
    bool                running;
    GameMode::Type      game_mode;
};

#endif
