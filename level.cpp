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

void
Level::Draw(Game *g) {
    int e2, i2;
    Item *item;
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

    for (std::list<Item*>::iterator it = this->items.begin();
            it != this->items.end(); it++) {
        item = &**it;
        ConditionallyShowObject(item->pos, item->symbol, item->colour);
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
    if (rand() % 100 < 50)
        this->AddGold(r);

    //Randomly add potions
    if (rand() % 100 < 20)
        this->AddPotion(r);

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
    Treasure *tres;
    int quantity;
    Point p;

    quantity = rand() % ((this->depth * this->depth) + 5) + 1;

    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
    } while (this->GetItem(p));

    tres = new Treasure(quantity);
    tres->SetPosition(p.x, p.y);

    this->items.push_back((Item*) tres);
}

void
Level::AddPotion(Rect *r) {
    Potion *potion;
    Potion::Category category;
    Point p;

    category = (Potion::Category)(rand() % Potion::LAST_CATEGORY);

    do {
        p.x = r->x + rand() % r->w;
        p.y = r->y + rand() % r->h;
    } while (this->GetItem(p));

    potion = new Potion(Potion::MINOR, category);
    potion->SetPosition(p.x, p.y);

    this->items.push_back((Item*) potion);
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

    //Items
    for (std::list<Item*>::iterator it = this->items.begin();
            it != this->items.end(); it++) {
        if (p == (*it)->pos) {
            ss << " and " << (*it)->GetName();
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
    Direction::Type tries[3];
    Tile *t;

    while(e->mv_energy >= e->mv_cost) {
        e->mv_energy -= e->mv_cost;

        //Target x
        if (e->pos.x > c->pos.x) {
            tries[0] = Direction::WEST;
            tries[1] = Direction::WEST;
        }
        else if (e->pos.x < c->pos.x) {
            tries[0] = Direction::EAST;
            tries[1] = Direction::EAST;
        }
        else {
            tries[0] = Direction::LAST_DIRECTION;
            if (rand() % 2)
                tries[1] = Direction::EAST;
            else
                tries[1] = Direction::WEST;
        }

        //Target y
        if (e->pos.y > c->pos.y) {
            tries[0] = MoveCardinal(Direction::NORTH, tries[0]);
            if (ABS(e->pos.x - c->pos.x) < ABS(e->pos.y - c->pos.y)) {
                tries[2] = tries[1];
                tries[1] = Direction::NORTH;
            }
            else
                tries[2] = Direction::NORTH;
        }
        else if (e->pos.y < c->pos.y) {
            tries[0] = MoveCardinal(Direction::SOUTH, tries[0]);
            if (ABS(e->pos.x - c->pos.x) < ABS(e->pos.y - c->pos.y)) {
                tries[2] = tries[1];
                tries[1] = Direction::SOUTH;
            }
            else
                tries[2] = Direction::SOUTH;
        }
        else
            tries[2] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );

        if (tries[0] == Direction::LAST_DIRECTION)
            tries[0] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );
        if (tries[0] == tries[1])
            tries[1] = (Direction::Type)( rand() % Direction::LAST_DIRECTION );

        for (int i = 0; i < 3; ++i) {
            target = GetRelativePoint(tries[i], e->pos);

            if (target == c->pos) {
                e->Attack(c);
                return;
            }

            if (this->GetEnemy(target)) // Can't move into an occupied space
                continue;

            t = &this->tiles[target.x][target.y];

            if (t->c == FLOOR_CHAR || t->c == OPEN_DOOR_CHAR) {
                e->pos = target;
                return;
            }
        }
    }
}

Enemy*
Level::GetEnemy(Point p) {
    for (std::list<Enemy>::iterator it = this->enemies.begin();
            it != this->enemies.end(); it++) {
        if (it->pos == p)
            return &*it;
    }
    return NULL;
}

Item *
Level::GetItem(Point p) {
    for (std::list<Item*>::iterator it = this->items.begin();
            it != this->items.end(); it++) {
        if ((*it)->pos == p)
            return &**it;
    }
    return NULL;
}

Level::~Level(void) {
    for (std::list<Item*>::iterator it = this->items.begin();
            it != this->items.end(); it++) {
        delete &**it;
    }
    if (this->next)
        delete this->next;
}
