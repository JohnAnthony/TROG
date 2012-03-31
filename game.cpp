#include <ncurses.h>
#include "game.hpp"

Game::Game(void) {
    this->levels = this->cur_level = new Level(NULL);
    this->character = new Character();
    this->levels->character = character;
    this->character->MoveTo(this->cur_level->stairs_up);
    this->levels->CentreCam(character->pos);

    this->character->sight_range = 5;   //TEMPORARY

    this->levels->RevealSight(this->character);


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
        else if (c == 'i')
            new_gamemode = GameMode::INFO_SCREEN;
        else if (c == '@')
            new_gamemode = GameMode::CHARACTER_SCREEN;
        else if (c == '`')
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
        case (GameMode::INFO_SCREEN):
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
            else if (c == '8' || c == 'w')
                this->MoveCharacter(Direction::NORTH);
            else if (c == '2' || c == 's')
                this->MoveCharacter(Direction::SOUTH);
            else if (c == '6' || c == 'd')
                this->MoveCharacter(Direction::EAST);
            else if (c == '4' || c == 'a')
                this->MoveCharacter(Direction::WEST);
            else if (c == '7' || c == 'q')
                this->MoveCharacter(Direction::NW);
            else if (c == '9' || c == 'e')
                this->MoveCharacter(Direction::NE);
            else if (c == '1' || c == 'z')
                this->MoveCharacter(Direction::SW);
            else if (c == '3' || c == 'c')
                this->MoveCharacter(Direction::SE);
            break;
        case GameMode::INFO_SCREEN:
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

    this->character->MoveTo(this->cur_level->stairs_up);
    this->cur_level->RevealSight(this->character);
    this->cur_level->CentreCam(this->character->pos);
    this->DoRedraw();
}

void
Game::GoDownALevel(void) {
    if (!this->cur_level->next)
        this->cur_level->next = new Level(this->cur_level);

    this->cur_level = this->cur_level->next;

    this->character->MoveTo(this->cur_level->stairs_up);
    this->cur_level->RevealSight(this->character);
    this->cur_level->CentreCam(this->character->pos);
    this->DoRedraw();
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
        default:
            break;
    }

    this->DoRedraw();
}

void
Game::DoRedraw(void) {
    if (this->game_mode == GameMode::MAP_WALK) {
        this->cur_level->Draw();
    }
}

void
Game::MoveCharacter(Direction::Type d) {
    Point target;
    Character *c;
    Tile *t;

    c = this->character;
    target = c->pos;

    if (d == Direction::NORTH)
        target.y--;
    else if (d == Direction::SOUTH)
        target.y++;
    else if (d == Direction::EAST)
        target.x++;
    else if (d == Direction::WEST)
        target.x--;
    else if (d == Direction::NW) {
        target.x--;
        target.y--;
    }
    else if (d == Direction::NE) {
        target.x++;
        target.y--;
    }
    else if (d == Direction::SW) {
        target.x--;
        target.y++;
    }
    else { // South East
        target.x++;
        target.y++;
    }

    t = &this->cur_level->tiles[target.x][target.y];

    // OK, we're ready to attempt a move!
    if (t->c == FLOOR_CHAR || t->c == OPEN_DOOR_CHAR) {
        c->MoveTo(target);
        this->cur_level->RevealSight(c);
        this->DoRedraw();
    }
    else if (t->c == CLOSED_DOOR_CHAR) {
        if (BinaryChoice("This door is closed. Open?", 'y', 'n')) {
            t->c = OPEN_DOOR_CHAR;
            // c->TakeActionCost();
        }
        this->DoRedraw();
    }
}

Game::~Game(void) {
    delete character;
    if (levels)
        delete levels;
}
