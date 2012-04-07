#include <ncurses.h>
#include <sstream>
#include "game.hpp"

Game::Game(void) {
    this->levels = this->cur_level = new Level(NULL);
    this->character = new Character(Character::LIZARDFOLK, Character::FIGHTER);
    this->levels->character = character;
    this->character->MoveTo(this->cur_level->stairs_up);
    this->levels->CentreCam(character->pos);
    this->gold = 0;

    this->levels->RevealSight(this->character);

    this->MakeStatusLine();

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;
}

void
Game::MakeStatusLine(void) {
    std::stringstream s;
    Character *c;

    c = this->character;

    s << " HP:" << c->curHP << "/" << c->maxHP;
    s << " MP:" << c->curMP << "/" << c->maxMP;
    s << " GP:" << this->gold;

    this->status_line = s.str();
}

void
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = this->game_mode;
    this->cur_level->Draw(this);

    while (this->running) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(12)) {
            this->running = !BinaryChoice("Are you sure you want to quit?", 'y', 'n');
            if (this->running)
                this->DoRedraw();
        }
        else if (c == 'l')
            new_gamemode = GameMode::MAP_LOOK;
        else if (c == 'i')
            new_gamemode = GameMode::INVENTORY_SCREEN;
        else if (c == 'g')
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
            this->DoRedraw();
            this->MakeStatusLine();
            break;
        case (GameMode::MAP_LOOK):
            this->target = this->character->pos;
            this->DoRedraw();
            break;
        case (GameMode::INFO_SCREEN):
            this->ShowMapInfo();
            break;
        case (GameMode::CHARACTER_SCREEN):
            break;
        case (GameMode::INVENTORY_SCREEN):
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
            if (c == '>') {
                if (this->character->pos == this->cur_level->stairs_down)
                    this->GoDownALevel();
            }
            else if (c == '<') {
                if (this->character->pos == this->cur_level->stairs_up)
                    this->GoUpALevel();
            }
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
            else if (c == '2' || c == 'x')
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
            else if (c == ',')
                this->DoPickup();
            break;
        case GameMode::MAP_LOOK:
            if (c == KEY_UP)
                this->MoveCamera(Direction::NORTH);
            else if (c == KEY_DOWN)
                this->MoveCamera(Direction::SOUTH);
            else if (c == KEY_RIGHT)
                this->MoveCamera(Direction::EAST);
            else if (c == KEY_LEFT)
                this->MoveCamera(Direction::WEST);
            else if (c == '8' || c == 'w')
                this->MoveLookTarget(Direction::NORTH);
            else if (c == '2' || c == 'x')
                this->MoveLookTarget(Direction::SOUTH);
            else if (c == '6' || c == 'd')
                this->MoveLookTarget(Direction::EAST);
            else if (c == '4' || c == 'a')
                this->MoveLookTarget(Direction::WEST);
            else if (c == '7' || c == 'q')
                this->MoveLookTarget(Direction::NW);
            else if (c == '9' || c == 'e')
                this->MoveLookTarget(Direction::NE);
            else if (c == '1' || c == 'z')
                this->MoveLookTarget(Direction::SW);
            else if (c == '3' || c == 'c')
                this->MoveLookTarget(Direction::SE);
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

    this->character->MoveTo(this->cur_level->stairs_down);
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
        this->cur_level->CentreCam(this->character->pos);
        this->cur_level->Draw(this);
        this->RedrawStatus();
    }
    else if (this->game_mode == GameMode::MAP_LOOK){
        this->status_line = this->cur_level->DescriptionOfTile(this->target, this);
        this->cur_level->Draw(this);
        this->RedrawStatus();
        this->DrawLookTarget();
    }
    else if (this->game_mode == GameMode::INFO_SCREEN)
        this->ShowMapInfo();
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
        this->cur_level->CentreCam(c->pos);
    }
    else if (t->c == CLOSED_DOOR_CHAR) {
        if (BinaryChoice("This door is closed. Open?", 'y', 'n')) {
            t->c = OPEN_DOOR_CHAR;
            this->cur_level->RevealSight(c);
            // c->TakeActionCost();
        }
    }
    this->MakeStatusLine();
    this->DoRedraw();
    this->RedrawStatus();
}

void
Game::DrawLookTarget(void) {
    Point p;
    static Color col = COL_PURPLE;

    if (!this->cur_level->IsOnScreen(target))
        return;

    p.y = target.y;
    p.x = target.x - 1;
    this->DrawAsOverlay(p, '-', col);
    p.x = target.x + 1;
    this->DrawAsOverlay(p, '-', col);
    p.x = target.x;
    p.y = target.y - 1;
    this->DrawAsOverlay(p, '|', col);
    p.y = target.y + 1;
    this->DrawAsOverlay(p, '|', col);
}

void
Game::MoveLookTarget(Direction::Type d) {
    if (d == Direction::NORTH)
        --this->target.y;
    else if (d == Direction::SOUTH)
        ++this->target.y;
    else if (d == Direction::EAST)
        ++this->target.x;
    else if (d == Direction::WEST)
        --this->target.x;
    else if (d == Direction::NW) {
        --this->target.y;
        --this->target.x;
    }
    else if (d == Direction::NE) {
        --this->target.y;
        ++this->target.x;
    }
    else if (d == Direction::SW) {
        --this->target.x;
        ++this->target.y;
    }
    else if (d == Direction::SE) {
        ++this->target.x;
        ++this->target.y;
    }
    else
        return; //Given a bad direction

    this->cur_level->CentreCam(this->target);
    this->DoRedraw();
}

void
Game::DrawAsOverlay(Point p, char c, int col) {
    if (!this->cur_level->IsOnScreen(p))
        return;
    attron(COLOR_PAIR(col));
    mvaddch(p.y - this->cur_level->cam.y, p.x - this->cur_level->cam.x, c);
    attroff(COLOR_PAIR(col));
}

void
Game::RedrawStatus(void) {
    mvprintw(LINES -1, 0, this->status_line.c_str());
    for (int i = this->status_line.length(); i <= COLS; ++i)
        addch(' ');
}

void
Game::DoPickup(void) {
    GoldPile *gp;
    std::stringstream ss;
    std::list<GoldPile>::iterator it;

    for (it = this->cur_level->goldpiles.begin(), gp = NULL;
            it != this->cur_level->goldpiles.end(); ++it) {
        if (it->pos == this->character->pos) {
            gp = &*it;
            break;
        }
    }

    if (!gp) {
        this->status_line = "Nothing to pick up.";
    }
    else {
        this->gold += gp->quantity;
        ss << "Picked up " << gp->quantity << "gp.";
        this->status_line = ss.str();
        this->cur_level->goldpiles.erase(it);
    }

    this->RedrawStatus();
}

Game::~Game(void) {
    delete character;
    if (levels)
        delete levels;
}
