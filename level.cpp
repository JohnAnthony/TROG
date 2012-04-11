#include "level.hpp"
#include "gui.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <sstream>

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
Level::Draw(Game *g) {
    int e2, i2;
    Tile *t;
    char c;

    move(0,0);

    //Floor tiles
    for (int e = 0; e < LINES - 1; ++e) { //Save space for the status line
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

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR) {
                attron(COLOR_PAIR(COL_RED));
                addch(c);
                attroff(COLOR_PAIR(COL_RED));
            }
            else
                addch(c);
        }
    }

    for (std::list<GoldPile>::iterator it = this->goldpiles.begin();
            it != this->goldpiles.end(); it++) {
        ConditionallyShowObject(it->pos, '$', COL_YELLOW);
    }

    //Special objects
    ConditionallyShowObject(stairs_up, '<', COL_BLUE);
    ConditionallyShowObject(stairs_down, '>', COL_BLUE);

    //Enemies
    for (std::list<Enemy>::iterator it = this->enemies.begin();
            it != this->enemies.end(); it++) {
        ConditionallyShowObject(it->pos, it->symbol, it->colour);
    }

    //Character
    ConditionallyShowObject(character->pos, this->character->symbol,
        this->character->colour);

    //Tidy us back to white as default
    refresh();
}

void
Level::DrawObjectRelative(Point p, char c) {
    mvaddch(p.y - this->cam.y, p.x - this->cam.x, c);
}

void
Level::ApplyRoom(Room * const r) {
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

    //Randomly add gold
    if (rand() % 100 <= 50)
        this->AddGold(r);

    //Randomly add enemies
    if (rand() % 100 <= 70)
        this->EnemySpawn(r);

    //Handle recursively adding children
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
Level::CorridorFits(Corridor const * const c) {
    if (c->pos.x <= 1 || c->pos.y <= 1)
        return false;
    if (c->pos.x + c->pos.w >= MAP_W)
        return false;
    if (c->pos.y + c->pos.h >= MAP_H)
        return false;

    return true;
}

bool
Level::RoomFits(Room const * const r) {
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
Level::ApplyCorridor(Corridor const * const c) {
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
    if (p.x - this->cam.x >= COLS)
        return false;
    if (p.y - this->cam.y >= LINES - 1) // -1 gives space for the status bar
        return false;

    return true;
}

void
Level::ConditionallyShowObject(Point p, char c, int col) {
    if (!this->IsOnScreen(p))
        return;
    if (!this->TileIsVisible(p))
        return;
    attron(COLOR_PAIR(col));
    DrawObjectRelative(p, c);
    attroff(COLOR_PAIR(col));
}

void
Level::RevealSight(Entity *e) {
    DoSightBeam(Direction::NORTH, e->pos.x, e->pos.y, e->sight_range);
    DoSightBeam(Direction::SOUTH, e->pos.x, e->pos.y, e->sight_range);
    DoSightBeam(Direction::EAST, e->pos.x, e->pos.y, e->sight_range);
    DoSightBeam(Direction::WEST, e->pos.x, e->pos.y, e->sight_range);
}

void
Level::DoSightBeam(Direction::Type d, int x, int y, float ttl) {
    Tile *t;

    if (ttl < 0)
        return;
    t = &this->tiles[x][y];
    t->isVisible = true;
    if (!t->SeeThrough())
        return;

    if (d == Direction::NORTH) {
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::NORTH, x, y - 1, ttl - 1);
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
    }
    else if (d == Direction::SOUTH) {
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
        DoSightBeam(Direction::SOUTH, x, y + 1, ttl - 1);
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::EAST) {
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::EAST, x + 1, y, ttl - 1);
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::WEST) {
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
        DoSightBeam(Direction::WEST, x - 1, y, ttl - 1);
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
    }
    else if (d == Direction::NW)
        DoSightBeam(Direction::NW, x - 1, y - 1, ttl - 1.41);
    else if (d == Direction::NE)
        DoSightBeam(Direction::NE, x + 1, y - 1, ttl - 1.41);
    else if (d == Direction::SW)
        DoSightBeam(Direction::SW, x - 1, y + 1, ttl - 1.41);
    else if (d == Direction::SE)
        DoSightBeam(Direction::SE, x + 1, y + 1, ttl - 1.41);
}

void
Level::CentreCam(Point p) {
    this->cam.x = p.x - COLS / 2;
    this->cam.y = p.y - LINES / 2;
}

void
Level::AddGold(Rect *r) {
    GoldPile gp;
    gp.pos.x = r->x + rand() % r->w;
    gp.pos.y = r->y + rand() % r->h;
    gp.quantity = rand() % ((this->depth * this->depth) + 5) + 1;

    this->goldpiles.push_back(gp);
}

std::string
Level::DescriptionOfTile(Point p, Game *g) {
    Tile *t;
    std::stringstream ss;
    std::string s;
    static std::string unknown = "Unexplored space";
    static std::string prefix = "YOU SEE: ";

    if (p.x > MAP_W)
        return unknown;
    if (p.y > MAP_H)
        return unknown;

    t = &this->tiles[p.x][p.y];

    //Basic beginning part
    ss << prefix;

    // Tile information
    if (!t->isVisible)
        return unknown;
    else if (p == this->stairs_up)
        ss << "Stairs up";
    else if (p == this->stairs_down)
        ss << "Stairs down";
    else if (t->c == FLOOR_CHAR)
        ss << "Granite floor";
    else if (t->c == WALL_CHAR)
        return prefix + "A wall";
    else if (t->c == CLOSED_DOOR_CHAR )
        ss << "Closed door";
    else if (t->c == OPEN_DOOR_CHAR)
        ss << "Open door";
    else if (t->c == OPEN_DOOR_CHAR)
        ss << "Open door";

    //Character
    if (p == g->character->pos) {
        ss << " and a " << g->character->GetRaceString() << " ";
        ss << g->character->GetClassString();
    }
    else { // Check for enemies
        for (std::list<Enemy>::iterator it = this->enemies.begin();
                it != this->enemies.end(); it++) {
            if (it->pos == p) {
                ss << " and " << it->Description();
                break;
            }
        }
    }

    //Gold piles
    for (std::list<GoldPile>::iterator it = this->goldpiles.begin();
            it != this->goldpiles.end(); it++) {
        if (p == it->pos) {
            ss << " and " << it->quantity << "gp";
        }
    }

    s = ss.str();

    if (s.length() > 0)
        return s;

    return "SOMETHING WENT WRONG";
}

void
Level::EnemySpawn(Rect *r) {
    Enemy e;
    int type;

    type = rand() % LENGTH(EnemyList);

    e = EnemyList[type];
    e.pos.x = r->x + rand() % r->w;
    e.pos.y = r->y + rand() % r->h;

    if (e.pos == this->stairs_up)
        return;

    this->enemies.push_back(e);
}

void
Level::RemoveEnemy(Enemy *e) {
    for (std::list<Enemy>::iterator it = this->enemies.begin();
            it != this->enemies.end(); it++) {
        if (&*it == e) {
            this->enemies.erase(it);
            return;
        }
    }

    GUI::Alert("Error removing enemy from list. That's bad.");
}

void
Level::GiveEnemiesTurn(Character *c) {
    for (std::list<Enemy>::iterator it = this->enemies.begin();
            it != this->enemies.end(); it++) {
        if (!it->isActive && CalculateDistance(it->pos, c->pos) <= it->sight_range)
            it->isActive = true;
        if (it->isActive)
            it->mv_energy += c->mv_cost;
        if (it->mv_energy >= it->mv_cost)
            this->EnemyAdvance(&*it, c);
    }
}

void
Level::EnemyAdvance(Enemy *e, Character *c) {
    Point target;
    Tile *t;

    while(e->mv_energy >= e->mv_cost) {
        e->mv_energy -= e->mv_cost;

        //Target x
        if (c->pos.x > e->pos.x)
            target.x = e->pos.x + 1;
        else if (c->pos.x < e->pos.x)
            target.x = e->pos.x - 1;
        else
            target.x = e->pos.x;

        //Target y
        if (c->pos.y > e->pos.y)
            target.y = e->pos.y + 1;
        else if (c->pos.y < e->pos.y)
            target.y = e->pos.y - 1;
        else
            target.y = e->pos.y;

        if (target == c->pos) {
            e->Attack(c);
            return;
        }

        t = &this->tiles[target.x][target.y];

        if (t->c == FLOOR_CHAR || t->c == OPEN_DOOR_CHAR)
            e->pos = target;
    }
}
