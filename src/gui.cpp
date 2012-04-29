#include "gui.hpp"
#include "character.hpp"
#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <cstring>
#include <iostream>

#define NOISE_CHARACTER  '#'

Game *GUI::g;
Point GUI::cam;
int GUI::NUM_COLOURS;
std::string GUI::status_line;
std::vector<std::string> GUI::messages;
const char *GUI::SplashStr = "\
\n\
\n\
\n\
\n\
\n\
 .ddhddddmdddhhdddddm/                          ....              -oydNMMMNNmd: \
 :No:...`/MMM:.````.ysodNNdyyyhddy/       ./sddhyyydmmy+.        sMNs:.``.-omM: \
 +       -MMm``     ` `+MMy``  `oNh+.   .yMNo-```   `:hMMh:    :NMh.``      .m. \
         `mNM.         :MMs`     yMMs  .mMN:``         /NMM+  -NMm.`         s` \
         `hdM.         /MMs`     sMM+  hMMo`            +MMM: sMMs`             \
         .MMN.         /MMy`    :mNo   MMM+`             NMMs`mMM+`             \
         `MMN.         /MMdo++odMm-    MMMd`             mMMs`NMM+`    `:::/++  \
         .MMN`         /MMy-...oddh-   oMMM+            -MMN-`mMMs`     .-dMMm  \
         /MMM.         /MMy`    .yMN+   +NMNo          -mMd-` /MMm.       :MMy  \
         -MMM.         sMMh`     `oNMh:  .omMmo-....:+hmh+.`   hMMh`      -MMs  \
         +MMM`       :+ssso+:      .sNMh/- `-/ossyyyso/.``     `sMMy.     -MMs  \
         hMMM/                       `/+ss+.     ```             -hNNy+:-`.dMy  \
      .:oyyyhh/.                                                   ./oyyyys+:.  \
\n\
\n\
\n\
\n\
\n\
";

const char *GUI::TombStr = "\
                 ______________________________________________________\n\
                /                                                     /|\n\
               /                                                     / |\n\
              /                                                     /  |\n\
             /_____________________________________________________/   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |   |\n\
             |                                                     |  /\n\
             |                                                     | /\n\
             |                                                     |/\n\
            \\//__________\\//_________\\//_______\\//_____\\\\/________\\//\n\
";

void
GUI::Init(void) {
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    if (COLS < 80 || LINES < 24) {
        endwin();
        std::cout << "ERROR: This game requires a terminal size of at least 80x24\n";
        exit(1);
    }

    for (int i = 0; i < NUM_COLOURS; ++i) 
        init_pair(i, i, COLOR_BLACK);
}

void
GUI::End(void) {
    endwin();
}

void
GUI::AttachTo(Game *g) {
    GUI::g = g;
}

void
GUI::RedrawStatus(void) {
    mvprintw(LINES -1, 0, GUI::status_line.c_str());
    for (int i = GUI::status_line.length(); i <= COLS; ++i)
        addch(' ');
}

void
GUI::SetStatus(std::string str) {
    GUI::status_line = str;
    GUI::RedrawStatus();
}

void
GUI::AddMessage(std::string str) {
    messages.push_back(str);
}

void
GUI::ProcessMessages(Game *g) {
    std::string s;

    if (messages.size() == 0) {
        GUI::CharacterStatusLine(g->character);
        return;
    }

    for (std::vector<std::string>::iterator it = messages.begin(); 
            it != messages.end();) {
        s = *it;
        SetStatus(s);
        if (++it != messages.end()) {
            mvprintw(LINES - 1, s.length(), " <More>");
            getch();
        }
    }
    messages.clear();
}

Character*
GUI::CharacterCreation(void) {
    Character::Race race;
    Character::Class cclass;
    std::string name;

    GUI::ShowSplash();
    race = GUI::SelectRace();
    GUI::ShowSplash();
    cclass = GUI::SelectClass();
    GUI::ShowSplash();
    name = GUI::GetString("Please enter a name:");

    if (name == "") {
        GUI::Alert("You can't have no name. Setting your name to the default: 'Anonymous Trogger'.");
        name = "Anonymous Trogger";
    }

    return new Character(name, race, cclass);
}

Character::Race
GUI::SelectRace(void) {
    WINDOW *w;
    Rect pos;
    int ret;
    int c;
    static char const * const title =     ":: CHOOSE YOUR RACE ::";
    static char const * const underline = "======================";
    static char const * const left_ptr = ">>";
    static char const * const right_ptr = "<<";
    static char const * const blank = "  ";
    std::string s;

    //Determine how wide our window needs to be
    c = strlen(title) + 4;
    for (int i = 0; i < Character::LAST_RACE; ++i) {
        s = Character::RaceAsString((Character::Race) i );
        c = MAX((unsigned int)c, s.length() + strlen(left_ptr) + strlen(right_ptr) + 6);
    }

    pos.w = c;
    pos.h = Character::LAST_RACE*2 + 4;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for race selection!";
        exit(1);
    }
    box(w, 0, 0);

    mvwprintw(w, 0, (pos.w - strlen(title)) / 2, title);
    mvwprintw(w, 1, (pos.w - strlen(title)) / 2, underline);
    for (int i = 0; i < Character::LAST_RACE; ++i) {
        s = Character::RaceAsString((Character::Race) i );
        mvwprintw(w, 3 + i*2, 8, s.c_str());
    }
    wrefresh(w);

    //Menu driver
    ret = 0;
    c = 0;
    do {
        //Blank old position
        mvwprintw(w, 3 + ret*2, 2, blank);
        mvwprintw(w, 3 + ret*2, pos.w - strlen(right_ptr) - 2, blank);

        if (c == KEY_UP)
            ret = (ret == 0) ? Character::LAST_RACE - 1 : ret - 1;
        else if (c == KEY_DOWN)
            ret = (ret == Character::LAST_RACE - 1) ? 0 : ret + 1;

        //Redraw pointers
        mvwprintw(w, 3 + ret*2, 2, left_ptr);
        mvwprintw(w, 3 + ret*2, pos.w - strlen(right_ptr) - 2, right_ptr);
        wrefresh(w);
    } while ((c = getch()) != '\n');

    //Cleanup
    delwin(w);
    return (Character::Race) ret;
}

Character::Class
GUI::SelectClass(void) {
    WINDOW *w;
    Rect pos;
    int ret;
    int c;
    static char const * const title =     ":: CHOOSE YOUR CLASS ::";
    static char const * const underline = "======================";
    static char const * const left_ptr = ">>";
    static char const * const right_ptr = "<<";
    static char const * const blank = "  ";
    std::string s;

    //Determine how wide our window needs to be
    c = strlen(title) + 4;
    for (int i = 0; i < Character::LAST_CLASS; ++i) {
        s = Character::ClassAsString((Character::Class) i );
        c = MAX((unsigned int)c, s.length() + strlen(left_ptr) + strlen(right_ptr) + 6);
    }

    pos.w = c;
    pos.h = Character::LAST_CLASS*2 + 4;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for class selection!";
        exit(1);
    }
    box(w, 0, 0);

    mvwprintw(w, 0, (pos.w - strlen(title)) / 2, title);
    mvwprintw(w, 1, (pos.w - strlen(title)) / 2, underline);
    for (int i = 0; i < Character::LAST_CLASS; ++i) {
        s = Character::ClassAsString((Character::Class) i );
        mvwprintw(w, 3 + i*2, 8, s.c_str());
    }
    wrefresh(w);

    //Menu driver
    ret = 0;
    c = 0;
    do {
        //Blank old position
        mvwprintw(w, 3 + ret*2, 2, blank);
        mvwprintw(w, 3 + ret*2, pos.w - strlen(right_ptr) - 2, blank);

        if (c == KEY_UP)
            ret = (ret == 0) ? Character::LAST_CLASS - 1 : ret - 1;
        else if (c == KEY_DOWN)
            ret = (ret == Character::LAST_CLASS - 1) ? 0 : ret + 1;

        //Redraw pointers
        mvwprintw(w, 3 + ret*2, 2, left_ptr);
        mvwprintw(w, 3 + ret*2, pos.w - strlen(right_ptr) - 2, right_ptr);
        wrefresh(w);
    } while ((c = getch()) != '\n');

    //Cleanup
    delwin(w);
    return (Character::Class) ret;
}

std::string
GUI::GetString(std::string prompt) {
    static const int IN_SZ = 30;
    WINDOW *w;
    char in[IN_SZ];
    std::stringstream ss;
    std::string s;

    echo();
    curs_set(1);
    refresh();
    w = GUI::NewCentredWindow(IN_SZ + prompt.length() + 6, 3);
    if (!w) {
        std::cerr << "Error making window for string selection!";
        return "";
    }
    box(w, 0, 0);
    mvwprintw(w, 1, 2, prompt.c_str());
    waddch(w, ' ');

    wrefresh(w);
    refresh();
    wgetnstr(w, in, 30);

    noecho();
    curs_set(0);
    delwin(w);

    ss << in;
    s = ss.str();
    return s;
}


void
GUI::ShowSplash(void) {
    GUI::ShowSplash(COL_BLUE);
}

void
GUI::ShowSplash(Colour col) {
    WINDOW *w;

    refresh();

    w = GUI::NewCentredWindow(80, 24);
    wattron(w, COLOR_PAIR(col));
    mvwprintw(w, 0, 0, SplashStr);

    wattroff(w, COLOR_PAIR(col));
    wrefresh(w);
    refresh();
    delwin(w);
}

void
GUI::StartScreen(void) {
    static const std::string request = "Press any key to begin...";
    GUI::ShowSplash(COL_RED);
    mvprintw(LINES/2 + 10, ((COLS - request.length()) / 2), request.c_str());
    getch();
}

void
GUI::ScreenNoise(void) {
    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            mvaddch(e, i, '#'); //Magic number here is bad but efficient
        }
    }
}

WINDOW*
GUI::NewCentredWindow(int w, int h) {
    return newwin(h, w, (LINES - h) / 2, (COLS - w) / 2);
}

void
GUI::Alert(std::string str) {
    WINDOW *w;
    int c;
    Rect pos;
    static std::string const press_msg = "Press spacebar to continue.";

    pos.w = MAX(str.length(), press_msg.length()) + 4;
    pos.w = MIN(pos.w, 80);
    pos.h = 5;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for an alert!";
        return;
    }
    box(w, 0, 0);

    wrefresh(w);
    refresh();

    pos.x = (pos.w - str.length()) / 2;
    mvwprintw(w, 2, pos.x, str.c_str());
    pos.x = (pos.w - press_msg.length()) / 2;
    mvwprintw(w, 4, pos.x, press_msg.c_str());
    wrefresh(w);
    refresh();

    while((c = wgetch(w)) != ' ');

    delwin(w);
}

void
GUI::Alert2(char const * const * const msg) {
    WINDOW *w;
    int c;
    Rect pos;
    static std::string const press_msg = "Press spacebar to continue.";
    int maxlen;
    int lines;

    maxlen = press_msg.length();
    for (lines = 0; msg[lines]; ++lines) {
        if ((int)strlen(msg[lines]) > maxlen)
            maxlen = strlen(msg[lines]);
    }

    pos.w = MIN(maxlen + 4, 80);
    pos.h = 5 + lines - 1;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for an alert!";
        return;
    }
    box(w, 0, 0);

    wrefresh(w);
    refresh();

    for (int i = 0; i < lines; ++i) {
        pos.x = (pos.w - strlen(msg[i])) / 2;
        mvwprintw(w, 2+i, pos.x, msg[i]);
    }
    pos.x = (pos.w - press_msg.length()) / 2;
    mvwprintw(w, pos.h - 1, pos.x, press_msg.c_str());
    wrefresh(w);
    refresh();

    while((c = wgetch(w)) != ' ');

    delwin(w);
}

bool
GUI::BinaryChoice(std::string str, char a, char b) {
    WINDOW *w;
    Rect pos;
    char c;

    str.append(1, ' ');
    str.append(1, a);
    str.append(1, '/');
    str.append(1, b);

    pos.w = str.length() + 4;
    pos.h = 3;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for a binary choice! (Defaulted to true)";
        return true;
    }
    box(w, 0, 0);
    pos.x = (pos.w - str.length()) / 2;
    mvwprintw(w, 1, pos.x, str.c_str());

    wrefresh(w);

    while(1) {
        c = getch();

        if (c == a || c == b)
            break;
    }

    delwin(w);

    if (c == a)
        return true;
    return false;
}

void
GUI::FancyClear(void) {
    wmove(stdscr, 0, 0);
    for (int i = 0; i < LINES - 1; ++i) {
        for (int e = 0; e < COLS; ++e) {
            waddch(stdscr, ' ');
        }
    }
    refresh();
}

void
GUI::CharacterRename(Game *g) {
    std::string newname;
    Character *chr = g->character;

    newname = GUI::GetString("Enter the new character name: ");
    if (newname == "") {
        GUI::Alert("You can't have an empty name. Using the old name.");
        newname = g->character->name;
    }
    chr->name = newname;
}

void
GUI::ShowCharacterScreen(Character *c) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;
    Point p;

    pos.w = 80;
    pos.h = 24;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for character screen!";
        return;
    }
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
    ss << "HP  :: " << c->curHP << "/" << c->baseHP;
    s = ss.str();
    mvwprintw(w, p.y, p.x, s.c_str());

    ss.str("");
    ss << "MP  :: " << c->curMP << "/" << c->baseMP;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "STR :: " << c->curSTR;
    if (c->curSTR != c->baseSTR)
        ss << "\t(Base :: " << c->baseSTR << ")";
    s = ss.str();
    mvwprintw(w, p.y += 2, p.x, s.c_str());

    ss.str("");
    ss << "TOU :: " << c->curTOU;
    if (c->curTOU != c->baseTOU)
        ss << "\t(Base :: " << c->baseTOU << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "ATT :: " << c->curATT;
    if (c->curATT != c->baseATT)
        ss << "\t(Base :: " << c->baseATT << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "DEF :: " << c->curDEF;
    if (c->curDEF != c->baseDEF)
        ss << "\t(Base :: " << c->baseDEF << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "MAG :: " << c->curMAG;
    if (c->curMAG != c->baseMAG)
        ss << "\t(Base :: " << c->baseMAG << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    if (!w) {
        std::cerr << "Error making window for inventory screen!";
        return;
    }
    ss << "WIL :: " << c->curWIL;
    if (c->curWIL != c->baseWIL)
        ss << "\t(Base :: " << c->baseWIL << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    mvwprintw(w, p.y += 2, p.x -1, "=|==|==|==|==|==|==|==|==|==|==|==|=");

    ss.str("");
    ss << "SIGHT RANGE :: " << c->curSIGHT;
    if (c->curSIGHT != c->baseSIGHT)
        ss << "\t(Base :: " << c->baseSIGHT << ")";
    s = ss.str();
    mvwprintw(w, p.y += 2, p.x, s.c_str());

    ss.str("");
    ss << "MV COST     :: " << c->curMV;
    if (c->curMV != c->baseMV)
        ss << "\t(Base :: " << c->baseMV << ")";
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
    ss << "XP    :: " << c->getXP() << "/" << c->next_level;
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    mvwprintw(w, p.y + 2, p.x - 1, "=|==|==|==|==|==|==|==|==|==|==|==|==|=");

    //Second row. Equipment section.
    p.y = 10;
    ss.str("");
    ss << "HEAD   :: " << c->getEquipmentNameWithQuality(HEAD);
    s = ss.str();
    mvwprintw(w, p.y, p.x, s.c_str());

    ss.str("");
    ss << "BODY   :: " << c->getEquipmentNameWithQuality(BODY);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "WEAPON :: " << c->getEquipmentNameWithQuality(WEAPON);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "SHIELD :: " << c->getEquipmentNameWithQuality(SHIELD);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "GLOVES :: " << c->getEquipmentNameWithQuality(GLOVES);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "BOOTS  :: " << c->getEquipmentNameWithQuality(BOOTS);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "RING1  :: " << c->getEquipmentNameWithQuality(RING1);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "RING2  :: " << c->getEquipmentNameWithQuality(RING2);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    ss << "NECK   :: " << c->getEquipmentNameWithQuality(NECK);
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    //Output all done
    wrefresh(w);
    delwin(w);
}

void
GUI::ShowInventoryScreen(Character *c) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;
    Item* item;
    int i;

    pos.w = 80;
    pos.h = 24;
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

    ss << "Wealth :: " << c->getGold() << "gp";
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
GUI::CharacterStatusLine(Character *c) {
    std::stringstream ss;

    ss <<  "HP:" << c->curHP << "/" << c->baseHP;
    if (c->baseMP > 0)
        ss << "    MP:" << c->curMP << "/" << c->baseMP;
    ss << "    GP:" << c->getGold();
    ss << "    XP:" << c->getXP() << "/" << c->next_level;

    GUI::SetStatus(ss.str());
}

void
GUI::ShowInfoScreen(Game *g) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;

    pos.w = 80;
    pos.h = 24;
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
    ss << "Current Depth   :: " << g->cur_level->depth;
    mvwprintw(w, 3, 3, ss.str().c_str());
    ss.str("");
    ss << "Deepest Visited :: " << g->character->deepest_visited;
    mvwprintw(w, 4, 3, ss.str().c_str());

    wrefresh(w);
    delwin(w);
}

void
GUI::QuitDialogue(Game *g) {
    GUI::FancyClear();
    GUI::ShowSplash();
    g->running = !GUI::BinaryChoice("Are you sure you want to quit?", 'y', 'n');
    if (g->running)
        GUI::DoRedraw();
}

void
GUI::DrawAsOverlay(Point p, char c, int col) {
    if (!GUI::isOnScreen(p))
        return;
    attron(COLOR_PAIR(col));
    mvaddch(p.y - GUI::cam.y, p.x - GUI::cam.x, c);
    attroff(COLOR_PAIR(col));
}

bool
GUI::isOnScreen(Point p) {
    if (p.x - GUI::cam.x < 0)
        return false;
    if (p.y - GUI::cam.y < 0)
        return false;
    if (p.x - GUI::cam.x >= COLS)
        return false;
    if (p.y - GUI::cam.y >= LINES - 1) // -1 gives space for the status bar
        return false;

    return true;
}

void
GUI::MoveCamera(Direction::Type d) {
    int step_x;
    int step_y;

    step_x = MAX(COLS / 3, 1);
    step_y = MAX(LINES / 3, 1);

    switch (d) {
        case Direction::NORTH:
            GUI::cam.y -= step_y;
            break;
        case Direction::SOUTH:
            GUI::cam.y += step_y;
            break;
        case Direction::EAST:
            GUI::cam.x += step_x;
            break;
        case Direction::WEST:
            GUI::cam.x -= step_x;
            break;
        default:
            break;
    }

    GUI::DoRedraw();
}

void
GUI::DoRedraw(void) {
    if (g->game_mode == GameMode::MAP_WALK) {
        GUI::DrawLevel(g->cur_level);
        GUI::RedrawStatus();
    }
    else if (g->game_mode == GameMode::MAP_LOOK){
        GUI::SetStatus(g->cur_level->DescriptionOfTile(g->target, g));
        GUI::DrawLevel(g->cur_level);
        GUI::RedrawStatus();
        g->DrawLookTarget();
    }
    else if (g->game_mode == GameMode::INFO_SCREEN)
        GUI::ShowInfoScreen(g);
    else if (g->game_mode == GameMode::CHARACTER_SCREEN)
        GUI::ShowCharacterScreen(g->character);
    else if (g->game_mode == GameMode::INVENTORY_SCREEN)
        GUI::ShowInventoryScreen(g->character);
    else if (g->game_mode == GameMode::POTION_SELECT)
        g->PotionSelectMenu->Show();
    else if (g->game_mode == GameMode::READING_SELECT)
        g->BookSelectMenu->Show();
    else if (g->game_mode == GameMode::GEAR_SELECT)
        g->EquipSelectMenu->Show();
    else if (g->game_mode == GameMode::SPELL_SELECT)
        g->SpellSelectMenu->Show();
}

void
GUI::DrawLevel(Level *l) {
    int e2, i2;
    Item *item;
    Tile *t;
    char c;
    Colour colour;

    move(0,0);

    //Floor tiles
    for (int e = 0; e < LINES - 1; ++e) { //Save space for the status line
        for (int i = 0; i < COLS; ++i) {
            i2 = i + GUI::cam.x;
            e2 = e + GUI::cam.y;
            t = &l->tiles[i2][e2];
            if (i2 >= MAP_W || i2 < 0 || e2 >= MAP_H || e2 < 0)
                c = ' ';
            else if (!t->isVisible)
                c = ' ';
            else
                c = t->getTileType();

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR) {
                attron(COLOR_PAIR(COL_RED));
                addch(c);
                attroff(COLOR_PAIR(COL_RED));
            }
            else if (c == FLOOR_CHAR) {
                if (GUI::NUM_COLOURS < 16)
                    colour = COL_WHITE;
                else
                    colour = COL_GREY;
                attron(COLOR_PAIR(colour));
                addch(c);
                attroff(COLOR_PAIR(colour));
            }
            else
                addch(c);
        }
    }

    for (std::list<Item*>::iterator it = l->items.begin();
            it != l->items.end(); it++) {
        item = &**it;
        l->ConditionallyShowObject(item->pos, item->symbol, item->colour);
    }

    //Special objects
    l->ConditionallyShowObject(l->stairs_up, '<', COL_CYAN);
    l->ConditionallyShowObject(l->stairs_down, '>', COL_CYAN);

    //Enemies
    for (std::list<Enemy>::iterator it = l->enemies.begin();
            it != l->enemies.end(); it++) {
        l->ConditionallyShowObject(it->pos, it->getSymbol(), it->getColour());
    }

    //Character
    l->ConditionallyShowObject(GUI::g->character->pos, GUI::g->character->symbol,
        GUI::g->character->colour);

    refresh();
}

void
GUI::DrawObjectRelative(Point p, char c) {
    mvaddch(p.y - GUI::cam.y, p.x - GUI::cam.x, c);
}

void
GUI::CentreCam(Point p) {
    GUI::cam.x = p.x - COLS / 2;
    GUI::cam.y = p.y - LINES / 2;
}

void
GUI::InfoScreen(Item *i) {
    if (!i)
        return;

    switch (i->type) {
        case Item::POTION:
            GUI::InfoScreen((Potion*) i);
            return;
        case Item::STAT_TOME:
            GUI::InfoScreen((StatTome*) i);
            return;
        case Item::EQUIPPABLE:
            GUI::InfoScreen((Equippable*) i);
            return;
        case Item::LAST_TYPE:
        case Item::GENERIC:
        case Item::TREASURE_T:
            GUI::Alert("Asked to describe something but I don't know what it is!");
            return;
    }
}
void
GUI::InfoScreen(Equippable *e) {
    std::stringstream ss;
    int nummods;
    char **fullmsg;
    int i;

    if (!e)
        return;

    nummods = 0;
    if (e->modSTR)
        nummods++;
    if (e->modTOU)
        nummods++;
    if (e->modATT)
        nummods++;
    if (e->modDEF)
        nummods++;
    if (e->modMAG)
        nummods++;
    if (e->modWIL)
        nummods++;
    if (e->modSIGHT)
        nummods++;
    if (e->modMV)
        nummods++;

    fullmsg = new char* [nummods + 3];
    fullmsg[0] = (char*) e->GetLongDescription();
    fullmsg[1] = (char*) "";
    i = 2;
    if (e->modSTR) {
        ss.str("");
        ss << "STR :: " << e->modSTR;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modTOU) {
        ss.str("");
        ss << "TOU :: " << e->modTOU;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modATT) {
        ss.str("");
        ss << "ATT :: " << e->modATT;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modDEF) {
        ss.str("");
        ss << "DEF :: " << e->modDEF;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modMAG) {
        ss.str("");
        ss << "MAG :: " << e->modMAG;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modWIL) {
        ss.str("");
        ss << "WIL :: " << e->modWIL;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modSIGHT) {
        ss.str("");
        ss << "SIGHT :: " << e->modSIGHT;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    if (e->modMV) {
        ss.str("");
        ss << "MV :: " << e->modMV;
        fullmsg[i] = strdup(ss.str().c_str());
        i++;
    }
    fullmsg[nummods + 2] = NULL;

    GUI::Alert2(fullmsg);
    for (int i = 0; i < nummods; ++i)
        free(fullmsg[i+2]);
    delete fullmsg;
}

void
GUI::InfoScreen(Potion *p) {
    if (!p)
        return;
    GUI::Alert(p->GetLongDescription());
}

void
GUI::InfoScreen(Spell *sp) {
    if (!sp)
        return;
    GUI::Alert(sp->description);
}

void
GUI::InfoScreen(StatTome *st) {
    if (!st)
        return;
    GUI::Alert(st->GetLongDescription());
}

void
GUI::HardRedraw(void) {
    erase();
    clear();
    endwin();
    refresh();
    GUI::DoRedraw();
}

void
GUI::ShowTombstone(Game *g) {
    std::stringstream ss;
    std::string s;
    Character *c;
    WINDOW *w;

    c = g->character;
    w = GUI::NewCentredWindow(80, 24);
    wprintw(w, GUI::TombStr);

    s = "R . I . P .";
    mvwprintw(w, 6, (80 - s.length()) / 2, s.c_str());
    ss << c->name << ", " << c->GetRaceString() << " ";
    ss << c->GetClassString();
    s = ss.str();
    mvwprintw(w, 8, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "XP: " << c->getXP() << "\tLevel: " << c->Level;
    s = ss.str();
    mvwprintw(w, 9, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Monsters killed: " << c->monsters_killed;
    s = ss.str();
    mvwprintw(w, 11, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Deepest travelled: " << c->deepest_visited;
    s = ss.str();
    mvwprintw(w, 12, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Gold collected: " << c->total_gold_collected;
    s = ss.str();
    mvwprintw(w, 13, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Potions drunk: " << c->potions_drunk;
    s = ss.str();
    mvwprintw(w, 14, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Books read: " << c->books_read;
    s = ss.str();
    mvwprintw(w, 15, (80 - s.length()) / 2, s.c_str());

    if (c->toughest_defeated) {
        s = "Toughest monster defeated:";
        mvwprintw(w, 17, (80 - s.length()) / 2, s.c_str());
        s = c->toughest_defeated->name;
        mvwprintw(w, 18, (80 - s.length()) / 2, s.c_str());
    }

    GUI::FancyClear();
    GUI::SetStatus("");
    wrefresh(w);
    refresh();
    delwin(w);
}

Direction::Type
GUI::GetDirection(void) {
    int c;

    GUI::DrawLevel(g->cur_level);
    GUI::SetStatus("Cast in which direction?");
    while (1) {
        c = getch();
        switch (c) {
            case '7':
            case 'q':
                return Direction::NW;
            case '8':
            case 'w':
                return Direction::NORTH;
            case '9':
            case 'e':
                return Direction::NE;
            case '4':
            case 'a':
                return Direction::WEST;
            case '6':
            case 'd':
                return Direction::EAST;
            case '1':
            case 'z':
                return Direction::SW;
            case '2':
            case 'x':
                return Direction::SOUTH;
            case '3':
            case 'c':
                return Direction::SE;
            default:
                break;
        }
    }
    return Direction::LAST_DIRECTION;
}
