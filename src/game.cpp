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
#include "debug.hpp"

#define KEYS_INVENTORY          'i'
#define KEYS_LOOK               'l'
#define KEYS_INFO               '?'
#define KEYS_CHARACTER_SCREEN   '@'
#define KEYS_ESCAPE             '`'
#define KEYS_POTION_SELECT      'p'
#define KEYS_READING_SELECT     'r'
#define KEYS_GEAR_SELECT        'g'
#define KEYS_SPELL_SELECT       'm'

Game::Game(Character *c) {
    this->character = c;
    this->levels = this->cur_level = new Level(NULL);
    this->character->MoveTo(this->cur_level->stairs_up);
    GUI::CentreCam(character->pos);

    this->levels->RevealSight(this->character->pos, this->character->curSIGHT);
    GUI::SetStatus("Welcome to TROG! Try not to die.");

    this->game_mode = GameMode::MAP_WALK;
    this->running = true;

    this->PotionSelectMenu = new ScrollableMenu("Drink what?");
    this->BookSelectMenu = new ScrollableMenu("Read what?");
    this->EquipSelectMenu = new ScrollableMenu("Equipment");
    this->SpellSelectMenu = new ScrollableMenu("Cast which spell?");

    //Give the player a few starting potions for luck
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));
    this->character->ItemToInventory((Item*) new Potion(Potion::MINOR, Potion::HEALING));

    this->need_hard_redraw = false;

    GUI::AttachTo(this);
}

bool
Game::Run(void) {
    int c;
    GameMode::Type new_gamemode;

    new_gamemode = GameMode::LAST_MODE;
    GUI::DrawLevel(this->cur_level);

    while (this->running) {
        c = getch();

        //Handle mode changes
        if (c == KEY_F(12))
            GUI::QuitDialogue(this);
        else if (c == KEY_F(8))
            Debug::DebugConsole(this);
        else if (c == KEY_F(4)) {
            GUI::HardRedraw();
        }
        else if (c == KEYS_LOOK)
            new_gamemode = GameMode::MAP_LOOK;
        else if (c == KEYS_INVENTORY)
            new_gamemode = GameMode::INVENTORY_SCREEN;
        else if (c == KEYS_INFO)
            new_gamemode = GameMode::INFO_SCREEN;
        else if (c == KEYS_CHARACTER_SCREEN)
            new_gamemode = GameMode::CHARACTER_SCREEN;
        else if (c == KEYS_ESCAPE)
            new_gamemode = GameMode::MAP_WALK;
        else if (c == KEYS_POTION_SELECT)
            new_gamemode = GameMode::POTION_SELECT;
        else if (c == KEYS_READING_SELECT)
            new_gamemode = GameMode::READING_SELECT;
        else if (c == KEYS_GEAR_SELECT)
            new_gamemode = GameMode::GEAR_SELECT;
        else if (c == KEYS_SPELL_SELECT)
            new_gamemode = GameMode::SPELL_SELECT;
        else    // Not a mode change. Handle input based upon mode
            new_gamemode = this->HandleInput(c);

        if (new_gamemode == this->game_mode)
            new_gamemode = GameMode::MAP_WALK;

        if (new_gamemode != GameMode::LAST_MODE)
            this->SwitchGameMode(new_gamemode);

        if (this->game_mode == GameMode::MAP_WALK)
            GUI::ProcessMessages(this);

        if (!this->character->isAlive()) {
            running = false;
            GUI::Alert("You have died!");
        }

        if (this->need_hard_redraw) {
            this->need_hard_redraw = false;
            GUI::HardRedraw();
        }
    }

    GUI::ShowTombstone(this);
    getch();
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
        case GameMode::MAP_WALK:
            GUI::CentreCam(this->character->pos);
            GUI::DoRedraw();
            GUI::CharacterStatusLine(this->character);
            break;
        case GameMode::MAP_LOOK:
            this->target = this->character->pos;
            GUI::DoRedraw();
            break;
        case GameMode::INFO_SCREEN:
            GUI::ShowInfoScreen(this);
            break;
        case GameMode::CHARACTER_SCREEN:
            GUI::ShowCharacterScreen(this->character);
            break;
        case GameMode::INVENTORY_SCREEN:
            GUI::ShowInventoryScreen(this->character);
            break;
        case GameMode::POTION_SELECT:
            GUI::FancyClear();
            this->PotionSelectMenu->Reset();
            this->RepopulatePotionMenu();
            this->PotionSelectMenu->Show();
            break;
        case GameMode::READING_SELECT:
            GUI::FancyClear();
            this->BookSelectMenu->Reset();
            this->RepopulateBookMenu();
            this->BookSelectMenu->Show();
            break;
        case GameMode::GEAR_SELECT:
            GUI::FancyClear();
            this->EquipSelectMenu->Reset();
            this->RepopulateEquipMenu();
            this->EquipSelectMenu->Show();
            break;
        case GameMode::SPELL_SELECT:
            GUI::FancyClear();
            this->SpellSelectMenu->Reset();
            this->RepopulateSpellMenu();
            this->SpellSelectMenu->Show();
            break;
        case GameMode::LAST_MODE:
        default:
            GUI::Alert("Error: Asked to switch to an unknown game mode");
            break;
    }
}

GameMode::Type
Game::HandleInput(int c) {
    Spell *sp;

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
            break;
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
            else if (c == 'v') {
                c = this->PotionSelectMenu->Selection();
                GUI::InfoScreen( this->character->PotionFromInventory( c ) );
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                this->character->DrinkPotion(this->PotionSelectMenu->Selection());
                this->cur_level->GiveEnemiesTurns(this->character, 1);
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
            else if (c == 'v') {
                c = this->BookSelectMenu->Selection();
                GUI::InfoScreen( this->character->StatTomeFromInventory( c ) );
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                this->character->ReadBookOrScroll(this->BookSelectMenu->Selection());
                this->cur_level->GiveEnemiesTurns(this->character, 1);
                GUI::DoRedraw();
                return GameMode::MAP_WALK;
            }
            break;
        case GameMode::GEAR_SELECT:
            if (c == KEY_UP)
                this->EquipSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->EquipSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->EquipSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->EquipSelectMenu->PageDown();
            else if (c == 'v') {
                c = this->EquipSelectMenu->Selection();
                GUI::InfoScreen( this->character->EquippableFromInventory( c ) );
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                this->HandleEquipSelection(this->EquipSelectMenu->Selection());
                this->cur_level->GiveEnemiesTurns(this->character, 1);
                GUI::DoRedraw();
            }
            break;
        case GameMode::SPELL_SELECT:
            if (c == KEY_UP)
                this->SpellSelectMenu->PtrUp();
            else if (c == KEY_DOWN)
                this->SpellSelectMenu->PtrDown();
            else if (c == KEY_PPAGE)
                this->SpellSelectMenu->PageUp();
            else if (c == KEY_NPAGE)
                this->SpellSelectMenu->PageDown();
            else if (c == 'v') {
                c = this->SpellSelectMenu->Selection();
                sp = this->character->SpellFromList(c);
                GUI::InfoScreen(sp);
                GUI::DoRedraw();
            }
            else if (c == '\n') {
                c = this->SpellSelectMenu->Selection();
                sp = this->character->SpellFromList(c);
                if (!sp)
                    return GameMode::MAP_WALK;
                if ((int)sp->MPcost > this->character->curMP) {
                    GUI::Alert("Not enough mana to cast this spell");
                    GUI::DoRedraw();
                    break;
                }
                this->character->curMP -= sp->MPcost;
                sp->function(this);
                GUI::DoRedraw();
                return GameMode::MAP_WALK;
            }
            break;

        default:
            break;
    }
    return GameMode::LAST_MODE;
}

void
Game::GoUpALevel(void) {
    if (! this->cur_level->prev)
        return;

    this->cur_level = this->cur_level->prev;

    this->character->MoveTo(this->cur_level->stairs_down);
    this->cur_level->RevealSight(this->character->pos, this->character->curSIGHT);
    GUI::CentreCam(this->character->pos);
    GUI::DoRedraw();
}

void
Game::GoDownALevel(void) {
    if (!this->cur_level->next)
        this->cur_level->next = new Level(this->cur_level);

    this->cur_level = this->cur_level->next;
    this->character->deepest_visited = MAX(this->character->deepest_visited,
            this->cur_level->depth);

    this->character->MoveTo(this->cur_level->stairs_up);
    this->cur_level->RevealSight(this->character->pos, this->character->curSIGHT);
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
            this->DoAttack(this->character, &*it);            //We're attacking instead
            break;
        }
    }

    if (it != this->cur_level->enemies.end()) {                // We did combat
    }
    else if (t->getTileType() == FLOOR_CHAR || t->getTileType() == OPEN_DOOR_CHAR) { 
            c->MoveTo(target);
            this->cur_level->RevealSight(c->pos, c->curSIGHT);
            GUI::CentreCam(c->pos);
            this->cur_level->CheckForRoomText(c);
            this->ReportStandingOn();
    }
    else if (t->getTileType() == CLOSED_DOOR_CHAR) {           // A closed door
        if (GUI::BinaryChoice("This door is closed. Open?", 'y', 'n')) {
            t->setTileType(OPEN_DOOR_CHAR);
            this->cur_level->RevealSight(c->pos, c->curSIGHT);
            GUI::AddMessage("You push the door open with a creak.");
        }
    }
    else                                                        // Invalid move
        return;

    GUI::DoRedraw();
    this->cur_level->GiveEnemiesTurns(c, 1);
    GUI::DoRedraw();
}

void
Game::DrawLookTarget(void) {
    Point p;
    static Colour col = COL_PURPLE;

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
            this->character->GiveGold(((Treasure*)item)->quantity);
            delete (Treasure*) item;
        }
        else
            this->character->ItemToInventory(item);
        GUI::AddMessage(ss.str());
        this->cur_level->items.erase(it);
        this->cur_level->GiveEnemiesTurns(this->character, 1);
    }
}

void
Game::HandleResize(int signal) {
    refresh();
    GUI::DoRedraw();
}

void
Game::DoAttack(Character *c, Enemy *e) {
    std::stringstream ss;
    int dam;

    e->isActive = true;

    ss << c->name << " attacks "  << e->Description();

    // Handle the attack
    dam = rand() % c->curATT - rand() % e->parent_type->baseDEF;
    if (dam < 0)
        ss << " but misses";
    else {
        dam += rand() % (int)(c->curSTR * 1.5) - rand() % e->parent_type->baseTOU;
        if (dam <= 0)
            ss << " but fails to do any damage";
        else
            ss << " for " << dam << " damage";
    }

    // Handle the effects of the attack
    if (dam > 0) {
        e->TakeDamage(dam);
        if (!e->isAlive()) {
            c->monsters_killed ++;
            if (!c->toughest_defeated)
                c->toughest_defeated = e->parent_type;
            else if (e->parent_type->XP_value > c->toughest_defeated->XP_value)
                c->toughest_defeated = e->parent_type;
            ss << " ... and kills it!";
            this->cur_level->RemoveEnemy(e);
            c->GiveXP(e->parent_type->XP_value);
            // Don't need this.
            //Level::RedrawEnemy should just redraw the one tile
            //But this is the quick and dirty way
            GUI::DoRedraw(); 
        }
    }

    GUI::AddMessage(ss.str());
}

void
Game::DoMagicAttack(Character *c, Enemy *e) {
    std::stringstream ss;
    int dam;

    e->isActive = true;

    // Handle the attack
    dam = rand() % c->curMAG + rand() % c->curMAG - rand() % e->parent_type->baseWIL;
    if (dam <= 0) {
        ss << "Your magical attack fails to damage " << e->Description();
        GUI::AddMessage(ss.str());
        return;
    }

    // Handle the effects of the attack
    if (dam > 0) {
        ss << "Your magic does " << dam << " damage to " << e->Description();
        e->TakeDamage(dam);
        if (!e->isAlive()) {
            c->monsters_killed ++;
            if (!c->toughest_defeated)
                c->toughest_defeated = e->parent_type;
            else if (e->parent_type->XP_value > c->toughest_defeated->XP_value)
                c->toughest_defeated = e->parent_type;
            ss << " ... and kills it!";
            this->cur_level->RemoveEnemy(e);
            c->GiveXP(e->parent_type->XP_value);
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
    this->cur_level->GiveEnemiesTurns(this->character, 1);
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

void
Game::RepopulateEquipMenu(void) {
    Character *c;
    Item* item;
    std::stringstream ss;

    c = this->character;

    for (int i = 0; i < (int)LAST_EQUIP_LOCATION; ++i) {
        if (c->equipment[i]) {
            ss.str("");
            ss << "E:" << c->equipment[i]->getNameWithQuality();
            this->EquipSelectMenu->AddItem(ss.str());
        }
    }

    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::EQUIPPABLE)
            this->EquipSelectMenu->AddItem(item->GetName());
    }
}

void
Game::RepopulateSpellMenu(void) {
    Character *c;
    Spell *sp;

    c = this->character;

    for (std::list<Spell*>::iterator it = c->SpellList.begin();
            it != c->SpellList.end(); ++it) {
        sp = &**it;
        this->SpellSelectMenu->AddItem(sp->name);
    }
}

void
Game::HandleEquipSelection(int n) {
    std::list<Item*>::iterator it;
    Character *c;
    Equippable *e;
    Item *item;

    c = this->character;
    n++;

    for (int i = 0; i < (int)LAST_EQUIP_LOCATION; ++i) {
        if (c->equipment[i])
            --n;
        if (n == 0) {
            c->Unequip((EquipLocations) i);
            if (c->isEquipSlotFree((EquipLocations) i)) {
                this->EquipSelectMenu->Reset();
                this->RepopulateEquipMenu();
            }
            return;
        }
    }

    //get the item we want to equip
    for (it = c->Inventory.begin(); it != c->Inventory.end(); ++it) {
        item = &**it;
        if (item->type == Item::EQUIPPABLE)
            --n;
        if (n == 0) {
            e = (Equippable*) item;
            break;
        }
    }

    if (n != 0) {
        GUI::Alert("Error syncing up equip/deequip selection!");
        return;
    }

    if ( !c->Equip(e) )
        return;

    c->Inventory.erase(it);
    this->EquipSelectMenu->Reset();
    this->RepopulateEquipMenu();
}

void
Game::ReportStandingOn(void) {
    Item *item;

    item = this->cur_level->GetItem(this->character->pos);
    if (!item)
        return;

    GUI::AddMessage("STANDING ON: " + item->GetName());
}

Game::~Game(void) {
    if (this->levels)
        delete this->levels;
    delete this->PotionSelectMenu;
    delete this->BookSelectMenu;
    delete this->EquipSelectMenu;
    delete this->SpellSelectMenu;
}
