#include <ncurses.h>
#include "game.hpp"

Game::Game(void) {
    this->levels = this->cur_level = new Level(NULL);
    this->character = new Character();

    this->cam.x = 0;
    this->cam.y = 0;

    this->game_mode = GameMode::MAP_WALK;
}

void
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = this->game_mode;

    while (1) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(1))
            return;
        else if (c == 'm')
            new_gamemode = GameMode::MAP_INFO;
        else if (c == 'c')
            new_gamemode = GameMode::CHARACTER_SCREEN;
        else if (c == 'z')
            new_gamemode = GameMode::MAP_WALK;
        else    // Not a mode change. Handle input based upon mode
            this->HandleInput(c);

        if (new_gamemode != this->game_mode)
            this->SwitchGameMode(new_gamemode);
    }
}

void
Game::SwitchGameMode(GameMode::Type gmt) {
    this->game_mode = gmt;

    switch (gmt) {
        case (GameMode::MAP_WALK):
            this->ShowMap();
            break;
        case (GameMode::MAP_INFO):
            this->ShowMapInfo();
            break;
        case (GameMode::CHARACTER_SCREEN):
            break;
    }
}

void
Game::ShowMap(void) {
    int e2, i2;
    char c;

    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            i2 = i + this->cam.x;
            e2 = e + this->cam.y;
            if (i2 >= MAP_W || i2 < 0 || e2 >= MAP_H || e2 < 0)
                c = 'X';
            else
                c = cur_level->tiles[i2][e2].c;

            mvaddch(e, i, c);
        }
    }

    refresh();
}

void
Game::ShowMapInfo(void) {
    ScreenNoise();
    mvprintw(0, 0, "Level: %d", this->cur_level->depth);
}

void
Game::HandleInput(int c) {
    switch (this->game_mode) {
        case GameMode::MAP_WALK:
            if (c == '>')
                this->GoDownALevel();
            else if (c == '<')
                this->GoUpALevel();
            else if (c == KEY_UP)
                this->MoveCamera(Direction::NORTH);
            else if (c == KEY_DOWN)
                this->MoveCamera(Direction::SOUTH);
            else if (c == KEY_RIGHT)
                this->MoveCamera(Direction::EAST);
            else if (c == KEY_LEFT)
                this->MoveCamera(Direction::WEST);
            break;
        case GameMode::MAP_INFO:
        case GameMode::CHARACTER_SCREEN:
        default:
            break;
    }
}

void
Game::GoUpALevel(void) {
    if (! this->cur_level->prev)
        return;

    this->cur_level = this->cur_level->prev;

    ShowMap();
}

void
Game::GoDownALevel(void) {
    if (!this->cur_level->next)
        this->cur_level->next = new Level(this->cur_level);

    this->cur_level = this->cur_level->next;
    ShowMap();
}

void
Game::MoveCamera(Direction::Type d) {
    int step_x;
    int step_y;

    step_x = MAX(COLS / 3, 1);
    step_y = MAX(LINES / 3, 1);

    switch (d) {
        case Direction::NORTH:
            this->cam.y -= step_y;
            break;
        case Direction::SOUTH:
            this->cam.y += step_y;
            break;
        case Direction::EAST:
            this->cam.x += step_x;
            break;
        case Direction::WEST:
            this->cam.x -= step_x;
            break;
    }

    this->ShowMap();
}

Game::~Game(void) {
    delete character;
    if (levels)
        delete levels;
}
