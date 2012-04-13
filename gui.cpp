#include "gui.hpp"
#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <cstring>

#define NOISE_CHARACTER  '#'

std::string GUI::status_line;
std::list<std::string> GUI::messages;
const char *GUI::SplashStr = "\
\n\
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
        g->CharacterStatusLine();
        return;
    }

    for (std::list<std::string>::iterator it = messages.begin(); 
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
GUI::ShowSplash(Color col) {
    WINDOW *w;

    refresh();

    w = GUI::NewCentredWindow(80, 25);
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
    pos.h = 5;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
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
