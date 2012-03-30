#include "level.hpp"
#include <cstdlib>
#include <ncurses.h>

#define CORRIDOR_TRIES      30

Point Level::cam;
Character* Level::character;

Level::Level(Level* parent) {
    Room r;

    this->prev = parent;
    this->next = NULL;

    if (this->prev)
        this->depth = this->prev->depth + 1;
    else
        this->depth = 1;

    //Blank everything
    for (int e = 0; e < MAP_H; ++e) {
        for (int i = 0; i < MAP_W; ++i) {
            this->tiles[i][e] = Tile(false, WALL_CHAR);
        }
    }

    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;
    r.x = rand() % (MAP_W - r.w - 2) + 1;
    r.y = rand() % (MAP_H - r.h - 2) + 1;

    this->stairs_up.x = r.x + rand() % (r.w - 2) + 1;
    this->stairs_up.y = r.y + rand() % (r.h - 2) + 1;
    this->stairs_down.x = -1;   //While this is -1 it means we haven't placed it

    this->ApplyRoom(&r);
}

Level::~Level(void) {
    if (this->next)
        delete this->next;
}

void
Level::Draw(void) {
    int e2, i2;
    Tile *t;
    char c;

    move(0,0);

    //Floor tiles
    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            i2 = i + this->cam.x;
            e2 = e + this->cam.y;
            t = &this->tiles[i2][e2];
            if (i2 >= MAP_W || i2 < 0 || e2 >= MAP_H || e2 < 0)
                c = ' ';
            else if (!t->isVisible)
                c = ' ';
            else
                c = t->c;

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR)
                attron(COLOR_PAIR(2));
            else
                attron(COLOR_PAIR(1));

            addch(c);
        }

        //Special objects
        attron(COLOR_PAIR(1));
    }

    ConditionallyShowObject(stairs_up, '<');
    ConditionallyShowObject(stairs_down, '>');
    ConditionallyShowObject(character->pos, '@');

    refresh();
}

void
Level::DrawObjectRelative(Point p, char c) {
    mvaddch(p.y - this->cam.y, p.x - this->cam.x, c);
}

void
Level::ApplyRoom(Room *r) {
    Corridor c;
    Room r_child;
    int exits;

    exits = 0;

    //Floor
    for (int e = 0; e < r->h; ++e) {
        for (int i = 0; i < r->w; ++i) {
            this->tiles[ i + r->x ][ e + r->y ].c = FLOOR_CHAR;
        }
    }

    for (int i = 0; i < CORRIDOR_TRIES; ++i) {
        c = this->FindRoomCorridorChild(r);
        if (!this->CorridorFits(&c))
            continue;
        r_child = this->RoomFromCorridor(&c);
        if (!this->RoomFits(&r_child))
            continue;

        exits++;
        this->ApplyCorridor(&c);
        this->ApplyRoom(&r_child);
    }

    if (this->stairs_down.x == -1 && exits == 0) { // We need to place stairs here
        this->stairs_down.x = r->x + rand() % (r->w - 2) + 1;
        this->stairs_down.y = r->y + rand() % (r->h - 2) + 1;
    }
}

bool
Level::CorridorFits(Corridor *c) {
    if (c->pos.x <= 1 || c->pos.y <= 1)
        return false;
    if (c->pos.x + c->pos.w >= MAP_W)
        return false;
    if (c->pos.y + c->pos.h >= MAP_H)
        return false;

    return true;
}

bool
Level::RoomFits(Room *r) {
    if (r->x <= 0 || r->y <= 0)
        return false;
    if (r->x + r->w >= MAP_W)
        return false;
    if (r->y + r->h >= MAP_H)
        return false;

    for (int e = MAX(r->y - 1, 0); e <= r->y + r->h; ++e) {
        for (int i = MAX(r->x - 1, 0); i <= r->x + r->w; ++i) {
            if (this->tiles[i][e].c == FLOOR_CHAR)
                return false;
        }
    }

    return true;
}

void
Level::ApplyCorridor(Corridor *c) {
    this->tiles[c->pos.x][c->pos.y].c = CLOSED_DOOR_CHAR;
    this->tiles[c->pos.x + c->pos.w][c->pos.y + c->pos.h].c = CLOSED_DOOR_CHAR;
    if (c->direction == Direction::EAST || c->direction == Direction::WEST) {
        for (int i = 1; i < c->pos.w; ++i)
            this->tiles[i + c->pos.x ][c->pos.y].c = FLOOR_CHAR;
    }
    else {
        for (int i = 1; i < c->pos.h; ++i)
            this->tiles[c->pos.x][ i + c->pos.y ].c = FLOOR_CHAR;
    }
}

Room
Level::RoomFromCorridor(Corridor *c) {
    Room r;
    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;

    // Only add-sized rooms
    if (r.w % 2 == 0)
        r.w++;
    if (r.h % 2 == 0)
        r.h++;

    if (c->direction == Direction::NORTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y - r.h;
    }
    else if (c->direction == Direction::SOUTH) {
        r.x = c->pos.x - rand() % r.w;
        r.y = c->pos.y + c->pos.h + 1;
    }
    else if (c->direction == Direction::EAST) {
        r.x = c->pos.x + c->pos.w + 1;
        r.y = c->pos.y - rand() % r.h;
    }
    else { //WEST
        r.x = c->pos.x - r.w;
        r.y = c->pos.y - rand() % r.h;
    }

    return r;
}

Corridor
Level::FindRoomCorridorChild(Room *r) {
    Corridor c;

    if (rand() % 2) { //NORTH OR SOUTH
        c.pos.w = 0;
        c.pos.h = ((rand() % 2) + 1) * 3;
        c.pos.x = r->x + rand() % r->w;
        if (rand() % 2) { //NORTH
            c.direction = Direction::NORTH;
            c.pos.y = r->y - 1 - c.pos.h;
        }
        else { //SOUTH
            c.direction = Direction::SOUTH;
            c.pos.y = r->y + r->h;
        }
    }
    else { //EAST OR WEST
        c.pos.h = 0;
        c.pos.w = ((rand() % 3) + 1) * 3;
        c.pos.y = r->y + rand() % r->h;
        if (rand() % 2) { // WEST
            c.direction = Direction::WEST;
            c.pos.x = r->x - 1 - c.pos.w;
        }
        else { //EAST
            c.direction = Direction::EAST;
            c.pos.x = r->x + r->w;
        }
    }

    return c;
}

bool
Level::TileIsVisible(Point p) {
    return this->tiles[p.x][p.y].isVisible;
}

bool
Level::IsOnScreen(Point p) {
    if (p.x - this->cam.x < 0)
        return false;
    if (p.y - this->cam.y < 0)
        return false;
    if (p.x - this->cam.x > COLS)
        return false;
    if (p.y - this->cam.y < 0)
        return false;

    return true;
}

void
Level::ConditionallyShowObject(Point p, char c) {
    if (!this->IsOnScreen(p))
        return;
    if (!this->TileIsVisible(p))
        return;
    DrawObjectRelative(p, c);
}

void
Level::RevealSight(Entity *e) {
    for (int y = MAX(e->pos.y - e->sight_range, 0); y < MIN(e->pos.y + e->sight_range, MAP_H); ++y) {
        for (int x = MAX(e->pos.x - e->sight_range, 0); x < MIN(e->pos.x + e->sight_range, MAP_W); ++x) {            
            this->tiles[x][y].isVisible = true;
        }
    }
}

void
Level::CentreCam(Point p) {
    this->cam.x = 0;
    this->cam.y = 0;
}
