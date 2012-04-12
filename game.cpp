#include <ncurses.h>
#include <cstdlib>
#include <sstream>
#include <list>
#include "game.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include "potion.hpp"
#include "item.hpp"
#include "treasure.hpp"
#include "scrollable_menu.hpp"

Game::Game(Character *c) {
    this->character = c;
    this->levels = this->cur_level = new Level(NULL);
    this->levels->character = character;
    this->character->MoveTo(this->cur_level->stairs_up);
    this->levels->CentreCam(character->pos);
    this->gold = 0;

    this->levels->RevealSight(this->character);
    GUI::SetStatus("Welcome to TROG! Try not to die.");

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;

    this->PotionSelectMenu = new ScrollableMenu("Potion Select");

    //Give the player a few starting potions for luck
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));
    this->character->ItemToInventory((Item*) new Potion(Potion::LIGHT, Potion::HEALING));
}

void
Game::CharacterStatusLine(void) {
    std::stringstream ss;
    Character *c;

    c = this->character;

    ss <<  "HP:" << c->curHP << "/" << c->maxHP;
    if (c->maxMP > 0)
        ss << " MP:" << c->curMP << "/" << c->maxMP;
    ss << " GP:" << this->gold;

    GUI::SetStatus(ss.str());
}

bool
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = this->game_mode;
    this->cur_level->Draw(this);

    while (this->running) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(12))
            this->QuitDialogue();
        else if (c == 'l')
            new_gamemode = GameMode::MAP_LOOK;
        else if (c == 'i')
            new_gamemode = GameMode::INVENTORY_SCREEN;
        else if (c == '?')
            new_gamemode = GameMode::INFO_SCREEN;
        else if (c == '@')
            new_gamemode = GameMode::CHARACTER_SCREEN;
        else if (c == '`')
            new_gamemode = GameMode::MAP_WALK;
        else if (c == 'p')
            new_gamemode = GameMode::POTION_SELECT;
        else    // Not a mode change. Handle input based upon mode
            new_gamemode = this->HandleInput(c);

        if (new_gamemode != this->game_mode)
            this->SwitchGameMode(new_gamemode);

        if (this->game_mode == GameMode::MAP_WALK)
            GUI::ProcessMessages(this);

        if (!this->character->isAlive()) {
            running = false;
            GUI::Alert("You have died!");
        }
    }

    return GUI::BinaryChoice("Do you wish to play again?", 'y', 'n');
}

void
Game::SwitchGameMode(GameMode::Type gmt) {
    if(this->game_mode == gmt)
        return;

    this->game_mode = gmt;

    switch (gmt) {
        case (GameMode::MAP_WALK):
            this->cur_level->CentreCam(this->character->pos);
            this->DoRedraw();
            this->CharacterStatusLine();
            break;
        case (GameMode::MAP_LOOK):
            this->target = this->character->pos;
            this->DoRedraw();
            break;
        case (GameMode::INFO_SCREEN):
            this->ShowInfoScreen();
            break;
        case (GameMode::CHARACTER_SCREEN):
            this->ShowCharacterScreen();
            break;
        case (GameMode::INVENTORY_SCREEN):
            this->ShowInventoryScreen();
            break;
        case (GameMode::POTION_SELECT):
            this->PotionSelectMenu->Reset();
            this->RepopulatePotionMenu();
            this->PotionSelectMenu->Show();
            break;
    }
}

GameMode::Type
Game::HandleInput(int c) {

    switch (this->game_mode) {
        case GameMode::MAP_WALK:
            if (c == '>') {
                if (this->character->pos == this->cur_level->stairs_down)
                    this->GoDownALevel();
                    GUI::AddMessage("You descend deeper...");
                    GUI::RedrawStatus();
            }
            else if (c == '<') {
                if (this->character->pos == this->cur_level->stairs_up)
                    this->GoUpALevel();
                    GUI::AddMessage("You ascend the stairs.");
                    GUI::RedrawStatus();
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
            else if (c == 's' || c == '5')
                this->DoWait();
            else if (c == ',')
                this->DoPickup();
            else if (c == '\n')
                GUI::RedrawStatus();
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
            break;
        case GameMode::POTION_SELECT:
            if (c == KEY_UP)
                this->PotionSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->PotionSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->PotionSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->PotionSelectMenu->PageDown();
            else if (c == '\n') {
                this->character->DrinkPotion(this->PotionSelectMenu->Selection());
                this->cur_level->GiveEnemiesTurn(this->character);
                this->DoRedraw();
                return GameMode::MAP_WALK;
            }
            break;
        default:
            break;
    }
    return this->game_mode;
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
        this->cur_level->Draw(this);
        GUI::RedrawStatus();
    }
    else if (this->game_mode == GameMode::MAP_LOOK){
        GUI::SetStatus(this->cur_level->DescriptionOfTile(this->target, this));
        this->cur_level->Draw(this);
        GUI::RedrawStatus();
        this->DrawLookTarget();
    }
    else if (this->game_mode == GameMode::INFO_SCREEN)
        this->ShowInfoScreen();
    else if (this->game_mode == GameMode::CHARACTER_SCREEN)
        this->ShowCharacterScreen();
    else if (this->game_mode == GameMode::INVENTORY_SCREEN)
        this->ShowInventoryScreen();
    else if (this->game_mode == GameMode::POTION_SELECT)
        this->PotionSelectMenu->Show();
}

void
Game::MoveCharacter(Direction::Type d) {
    Point target;
    Character *c;
    Tile *t;
    std::list<Enemy>::iterator it;

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

    //See if we're about to move on to an enemy
    for (it = this->cur_level->enemies.begin();
            it != this->cur_level->enemies.end(); it++) {
        if (target == it->pos) {
            this->DoAttack(this->character, &*it);              //We're attacking instead
            break;
        }
    }

    if (it != this->cur_level->enemies.end()) {                  // We did combat
    }
    else if (t->c == FLOOR_CHAR || t->c == OPEN_DOOR_CHAR) {    // An empty space
            c->MoveTo(target);
            this->cur_level->RevealSight(c);
            this->cur_level->CentreCam(c->pos);
    }
    else if (t->c == CLOSED_DOOR_CHAR) {                        // A closed door
        if (GUI::BinaryChoice("This door is closed. Open?", 'y', 'n')) {
            t->c = OPEN_DOOR_CHAR;
            this->cur_level->RevealSight(c);
            GUI::AddMessage("You push the door open with a creak.");
        }
    }
    else                                                        // Invalid move
        return;

    this->DoRedraw();
    this->cur_level->GiveEnemiesTurn(c);
    this->DoRedraw();
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
Game::DoPickup(void) {
    Item *item;
    std::stringstream ss;
    std::list<Item*>::iterator it;

    for (it = this->cur_level->items.begin(), item = NULL;
            it != this->cur_level->items.end(); ++it) {
        if ((*it)->pos == this->character->pos) {
            item = &**it;
            break;
        }
    }

    if (!item) {
        GUI::AddMessage("Nothing to pick up.");
    }
    else {
        ss << "You pick up " << item->GetName();;
        if (item->type == Item::TREASURE_T) {
            this->gold += ((Treasure*)item)->quantity;
            delete item;
        }
        else
            this->character->ItemToInventory(item);
        GUI::AddMessage(ss.str());
        this->cur_level->items.erase(it);
    }
}

void
Game::ShowCharacterScreen(void) {
    WINDOW *w;
    Rect pos;
    Character *c;
    std::stringstream ss;
    std::string s;
    Point p;

    c = this->character;

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    //Make our box shape
    //Horizontal lines
    wmove(w, 3, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, 0, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, pos.h - 1, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }

    //Actual output begins
    mvwprintw(w, 1, (pos.w - c->name.length()) / 2, c->name.c_str());

    s = c->GetRaceString() + " " + c->GetClassString() + " ";
    mvwprintw(w, 2, (pos.w - s.length()) / 2, s.c_str());

    p.x = 3;
    p.y = 5;
    ss.str("");
    ss << "HP  :: " << c->curHP << "/" << c->maxHP;
    s = ss.str();
    mvwprintw(w, p.y, p.x, s.c_str());

    ss.str("");
    ss << "MP  :: " << c->curMP << "/" << c->maxMP;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "STR :: " << c->curSTR;
    if (c->curSTR != c->maxSTR)
        ss << "/" << c->maxSTR;
    s = ss.str();
    mvwprintw(w, p.y += 2, p.x, s.c_str());

    ss.str("");
    ss << "TOU :: " << c->curTOU;
    if (c->curTOU != c->maxTOU)
        ss << "/" << c->maxTOU;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "ATT :: " << c->curATT;
    if (c->curATT != c->maxATT)
        ss << "/" << c->maxATT;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "DEF :: " << c->curDEF;
    if (c->curDEF != c->maxDEF)
        ss << "/" << c->maxDEF;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "MAG :: " << c->curMAG;
    if (c->curMAG != c->maxMAG)
        ss << "/" << c->maxMAG;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "WIL :: " << c->curWIL;
    if (c->curWIL != c->maxWIL)
        ss << "/" << c->maxWIL;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    mvwprintw(w, p.y += 2, p.x -1, "=|==|==|==|==|==|==|==|==|==|==|==|=");

    ss.str("");
    ss << "SIGHT RANGE :: " << c->sight_range;
    s = ss.str();
    mvwprintw(w, p.y += 2, p.x, s.c_str());

    ss.str("");
    ss << "MV COST     :: " << c->mv_cost;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    //Second row
    p.x = 40;
    p.y = 5;
    ss.str("");
    ss << "LEVEL :: " << c->Level;
    s = ss.str();
    mvwprintw(w, p.y, p.x, s.c_str());

    ss.str("");
    ss << "XP    :: " << c->XP << "/" << c->next_level;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    mvwprintw(w, p.y + 2, p.x - 1, "=|==|==|==|==|==|==|==|==|==|==|==|==|=");

    //Second row. Equipment section.
    p.y = 10;
    s = "HEAD   :: NONE";
    mvwprintw(w, p.y, p.x, s.c_str());

    s = "BODY   :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "WEAPON :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "HANDS  :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "FEET   :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "RING1  :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "RING2  :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    s = "NECK   :: NONE";
    mvwprintw(w, ++p.y, p.x, s.c_str());

    //Output all done
    wrefresh(w);
    delwin(w);
}

void
Game::ShowInventoryScreen(void) {
    WINDOW *w;
    Rect pos;
    Character *c;
    std::stringstream ss;
    std::string s;
    Item* item;
    int i;

    c = this->character;

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    //Make our box shape
    //Horizontal lines
    wmove(w, 0, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, 2, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, pos.h - 1, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }

    //header
    mvwprintw(w, 1, 2, "INVENTORY");

    ss << "Wealth :: " << this->gold << "gp";
    s = ss.str();
    mvwprintw(w, 1, pos.w - 2 - s.length(), s.c_str());

    i = 0;
    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it, ++i) {
        item = &**it;
        mvwprintw(w, i + 3, 8, item->GetName().c_str());
    }

    wrefresh(w);
    delwin(w);
}

void
Game::ShowInfoScreen(void) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    //Make our box shape
    //Horizontal lines
    wmove(w, 0, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, pos.h - 1, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }

    //header
    mvwprintw(w, 1, 2, "MAP INFO");

    //List of attributes
    ss << "Level :: " << this->cur_level->depth << " below ground";
    s = ss.str();
    mvwprintw(w, 3, 3, s.c_str());

    wrefresh(w);
    delwin(w);
}

void
Game::HandleResize(int signal) {
    refresh();
    this->DoRedraw();
}

void
Game::QuitDialogue(void) {
    this->running = !GUI::BinaryChoice("Are you sure you want to quit?", 'y', 'n');
    if (this->running)
        this->DoRedraw();
}

void
Game::DoAttack(Character *c, Enemy *e) { // Player -> Enemy version
    std::stringstream ss;
    int dam;

    ss << c->name << " attacks "  << e->Description();

    // Handle the attack
    dam = rand() % c->curATT - rand() % e->curDEF;
    if (dam < 0)
        ss << " but misses";
    else {
        dam += rand() % (c->curSTR * 2) - rand() % e->curTOU;
        if (dam <= 0)
            ss << " but fails to do any damage";
        else
            ss << " for " << dam << " damage";
    }

    // Handle the effects of the attack
    if (dam > 0) {
        e->TakeDamage(dam);
        if (e->curHP <= 0) {
            ss << " ... and kills it!";
            this->cur_level->RemoveEnemy(e);
            c->GiveXP(e->XP_value);
            // Don't need this.
            //Level::RedrawEnemy should just redraw the one tile
            //But this is the quick and dirty way
            this->DoRedraw(); 
        }
    }

    GUI::AddMessage(ss.str());
}

void
Game::DoWait(void) {
    this->cur_level->GiveEnemiesTurn(this->character);
    this->DoRedraw();
}

void
Game::RepopulatePotionMenu(void) {
    Character *c;
    Item* item;

    c = this->character;

    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::POTION)
            this->PotionSelectMenu->AddItem(item->GetName());
    }
}

Game::~Game(void) {
    if (levels)
        delete levels;
    delete PotionSelectMenu;
}
