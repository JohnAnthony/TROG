#include <cstdlib>
#include <sstream>
#include <list>
#include "game.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include "potion.hpp"
#include "item.hpp"
#include "treasure.hpp"
#include "stattome.hpp"
#include "scrollable_menu.hpp"

Game::Game(Character *c) {
    this->character = c;
    this->levels = this->cur_level = new Level(NULL);
    this->character->MoveTo(this->cur_level->stairs_up);
    GUI::CentreCam(character->pos);

    this->levels->RevealSight(this->character);
    GUI::SetStatus("Welcome to TROG! Try not to die.");

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;

    this->PotionSelectMenu = new ScrollableMenu("Drink what?");
    this->BookSelectMenu = new ScrollableMenu("Read what?");

    //Give the player a few starting potions for luck
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));


    //Debug
    // for (int i = 0; i < 400; ++i) {
    //     this->character->ItemToInventory((Item*) Equippable::RandomEquippable(1));
    //     this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));
    // }

    GUI::AttachTo(this);
}

bool
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = this->game_mode;
    GUI::DrawLevel(this->cur_level);

    while (this->running) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(12))
            GUI::QuitDialogue(this);
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
        else if (c == 'r')
            new_gamemode = GameMode::READING_SELECT;
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

    GUI::FancyClear();
    GUI::ShowSplash();
    return GUI::BinaryChoice("Do you wish to play again?", 'y', 'n');
}

void
Game::SwitchGameMode(GameMode::Type gmt) {
    if(this->game_mode == gmt)
        return;

    this->game_mode = gmt;

    switch (gmt) {
        case (GameMode::MAP_WALK):
            GUI::CentreCam(this->character->pos);
            GUI::DoRedraw();
            GUI::CharacterStatusLine(this->character);
            break;
        case (GameMode::MAP_LOOK):
            this->target = this->character->pos;
            GUI::DoRedraw();
            break;
        case (GameMode::INFO_SCREEN):
            GUI::ShowInfoScreen(this);
            break;
        case (GameMode::CHARACTER_SCREEN):
            GUI::ShowCharacterScreen(this->character);
            break;
        case (GameMode::INVENTORY_SCREEN):
            GUI::ShowInventoryScreen(this->character);
            break;
        case (GameMode::POTION_SELECT):
            GUI::FancyClear();
            GUI::ShowSplash();
            this->PotionSelectMenu->Reset();
            this->RepopulatePotionMenu();
            this->PotionSelectMenu->Show();
            break;
        case (GameMode::READING_SELECT):
            GUI::FancyClear();
            GUI::ShowSplash();
            this->BookSelectMenu->Reset();
            this->RepopulateBookMenu();
            this->BookSelectMenu->Show();
            break;
    }
}

GameMode::Type
Game::HandleInput(int c) {

    switch (this->game_mode) {
        case GameMode::MAP_WALK:
            if (c == '>') {
                if (this->character->pos == this->cur_level->stairs_down) {
                    this->GoDownALevel();
                    GUI::AddMessage("You descend deeper...");
                    GUI::RedrawStatus();
                }
            }
            else if (c == '<') {
                if (this->character->pos == this->cur_level->stairs_up) {
                    this->GoUpALevel();
                    GUI::AddMessage("You ascend the stairs.");
                    GUI::RedrawStatus();
                }
            }
            else if (c == KEY_UP)
                GUI::MoveCamera(Direction::NORTH);
            else if (c == KEY_DOWN)
                GUI::MoveCamera(Direction::SOUTH);
            else if (c == KEY_RIGHT)
                GUI::MoveCamera(Direction::EAST);
            else if (c == KEY_LEFT)
                GUI::MoveCamera(Direction::WEST);
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
                GUI::MoveCamera(Direction::NORTH);
            else if (c == KEY_DOWN)
                GUI::MoveCamera(Direction::SOUTH);
            else if (c == KEY_RIGHT)
                GUI::MoveCamera(Direction::EAST);
            else if (c == KEY_LEFT)
                GUI::MoveCamera(Direction::WEST);
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
                GUI::DoRedraw();
                return GameMode::MAP_WALK;
            }
            break;
        case GameMode::READING_SELECT:
            if (c == KEY_UP)
                this->BookSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->BookSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->BookSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->BookSelectMenu->PageDown();
            else if (c == '\n') {
                this->character->ReadBookOrScroll(this->BookSelectMenu->Selection());
                this->cur_level->GiveEnemiesTurn(this->character);
                GUI::DoRedraw();
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
    GUI::CentreCam(this->character->pos);
    GUI::DoRedraw();
}

void
Game::GoDownALevel(void) {
    if (!this->cur_level->next)
        this->cur_level->next = new Level(this->cur_level);

    this->cur_level = this->cur_level->next;

    this->character->MoveTo(this->cur_level->stairs_up);
    this->cur_level->RevealSight(this->character);
    GUI::CentreCam(this->character->pos);
    GUI::DoRedraw();
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
            GUI::CentreCam(c->pos);
            this->cur_level->CheckForRoomText(c);
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

    GUI::DoRedraw();
    this->cur_level->GiveEnemiesTurn(c);
    GUI::DoRedraw();
}

void
Game::DrawLookTarget(void) {
    Point p;
    static Color col = COL_PURPLE;

    if (!GUI::isOnScreen(target))
        return;

    p.y = target.y;
    p.x = target.x - 1;
    GUI::DrawAsOverlay(p, '-', col);
    p.x = target.x + 1;
    GUI::DrawAsOverlay(p, '-', col);
    p.x = target.x;
    p.y = target.y - 1;
    GUI::DrawAsOverlay(p, '|', col);
    p.y = target.y + 1;
    GUI::DrawAsOverlay(p, '|', col);
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

    GUI::CentreCam(this->target);
    GUI::DoRedraw();
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
            this->character->gold += ((Treasure*)item)->quantity;
            delete (Treasure*) item;
        }
        else
            this->character->ItemToInventory(item);
        GUI::AddMessage(ss.str());
        this->cur_level->items.erase(it);
        this->cur_level->GiveEnemiesTurn(this->character);
    }
}

void
Game::HandleResize(int signal) {
    refresh();
    GUI::DoRedraw();
}

void
Game::DoAttack(Character *c, Enemy *e) { // Player -> Enemy version
    std::stringstream ss;
    int dam;

    ss << c->name << " attacks "  << e->Description();

    // Handle the attack
    dam = rand() % c->curATT - rand() % e->DEF;
    if (dam < 0)
        ss << " but misses";
    else {
        dam += rand() % (int)(c->curSTR * 1.5) - rand() % e->TOU;
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
            GUI::DoRedraw(); 
        }
    }

    GUI::AddMessage(ss.str());
}

void
Game::DoWait(void) {
    this->cur_level->GiveEnemiesTurn(this->character);
    GUI::DoRedraw();
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

void
Game::RepopulateBookMenu(void) {
    Character *c;
    Item* item;

    c = this->character;

    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::STAT_TOME)
            this->BookSelectMenu->AddItem(item->GetName());
    }
}

Game::~Game(void) {
    if (this->levels)
        delete this->levels;
    delete this->PotionSelectMenu;
    delete this->BookSelectMenu;
}
