#include <ncurses.h>
#include "game.hpp"

Game::Game(void) {
    this->levels = this->cur_level = new Level(NULL);
    this->character = new Character();

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;
}

void
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = this->game_mode;
    this->cur_level->Draw();

    while (this->running) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(12)) {
            this->running = !BinaryChoice("Are you sure you want to quit?", 'y', 'n');
            if (this->running)
                this->DoRedraw();
        }
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
    if(this->game_mode == gmt)
        return;

    this->game_mode = gmt;

    switch (gmt) {
        case (GameMode::MAP_WALK):
            this->cur_level->Draw();
            break;
        case (GameMode::MAP_INFO):
            this->ShowMapInfo();
            break;
        case (GameMode::CHARACTER_SCREEN):
            break;
    }
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

    this->cur_level->cam.x = 0;
    this->cur_level->cam.y = 0;
    this->cur_level->Draw();
}

void
Game::GoDownALevel(void) {
    if (!this->cur_level->next)
        this->cur_level->next = new Level(this->cur_level);

    this->cur_level = this->cur_level->next;

    this->cur_level->cam.x = 0;
    this->cur_level->cam.y = 0;
    this->cur_level->Draw();
}

void
Game::MoveCamera(Direction::Type d) {
    int step_x;
    int step_y;
    Level *l = this->cur_level;

    step_x = MAX(COLS / 3, 1);
    step_y = MAX(LINES / 3, 1);

    switch (d) {
        case Direction::NORTH:
            l->cam.y -= step_y;
            break;
        case Direction::SOUTH:
            l->cam.y += step_y;
            break;
        case Direction::EAST:
            l->cam.x += step_x;
            break;
        case Direction::WEST:
            l->cam.x -= step_x;
            break;
    }

    l->Draw();
}

void
Game::DoRedraw(void) {
    if (this->game_mode == GameMode::MAP_WALK)
        this->cur_level->Draw();
}

Game::~Game(void) {
    delete character;
    if (levels)
        delete levels;
}
