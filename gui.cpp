#include "gui.hpp"
#include <ncurses.h>
#include <menu.h>
#include <sstream>

#define NOISE_CHARACTER  '#'

std::string GUI::status_line;
std::list<std::string> GUI::messages;
const char *GUI::SplashStr = "\
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

    race = GUI::SelectRace();
    cclass = GUI::SelectClass();
    name = GUI::GetString("Please enter a name:");

    return new Character(name, race, cclass);
}

Character::Race
GUI::SelectRace(void) {
    WINDOW *w;
    Rect pos;
    int ret;
    int c;
    static const std::string title =     ":: CHOOSE YOUR RACE ::";
    static char const * const underline = "======================";
    static const std::string left_ptr = ">>";
    static const std::string right_ptr = "<<";
    static char const * const blank = "  ";
    std::string s;

    GUI::ShowSplash();

    //Determine how wide our window needs to be
    c = title.length() + 4;
    for (int i = 0; i < Character::LAST_RACE; ++i) {
        s = Character::RaceAsString((Character::Race) i );
        c = MAX(c, (int)s.length() + (int)left_ptr.length() + (int)right_ptr.length() + 6);
    }

    pos.w = c;
    pos.h = Character::LAST_RACE*2 + 4;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    mvwprintw(w, 0, (pos.w - title.length()) / 2, title.c_str());
    mvwprintw(w, 1, (pos.w - title.length()) / 2, underline);
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
        mvwprintw(w, 3 + ret*2, pos.w - right_ptr.length() - 2, blank);

        if (c == KEY_UP)
            ret = (ret == 0) ? Character::LAST_RACE - 1 : ret - 1;
        else if (c == KEY_DOWN)
            ret = (ret == Character::LAST_RACE - 1) ? 0 : ret + 1;

        //Redraw pointers
        mvwprintw(w, 3 + ret*2, 2, left_ptr.c_str());
        mvwprintw(w, 3 + ret*2, pos.w - right_ptr.length() - 2, right_ptr.c_str());
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
    static const std::string title =     ":: CHOOSE YOUR CLASS ::";
    static char const * const underline = "=======================";
    static const std::string left_ptr = ">>";
    static const std::string right_ptr = "<<";
    static char const * const blank = "  ";
    std::string s;

    GUI::ShowSplash();

    //Determine how wide our window needs to be
    c = title.length() + 4;
    for (int i = 0; i < Character::LAST_CLASS; ++i) {
        s = Character::ClassAsString((Character::Class) i );
        c = MAX(c, (int)s.length() + (int)left_ptr.length() + (int)right_ptr.length() + 6);
    }

    pos.w = c;
    pos.h = Character::LAST_CLASS*2 + 4;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    mvwprintw(w, 0, (pos.w - title.length()) / 2, title.c_str());
    mvwprintw(w, 1, (pos.w - title.length()) / 2, underline);
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
        mvwprintw(w, 3 + ret*2, pos.w - right_ptr.length() - 2, blank);

        if (c == KEY_UP)
            ret = (ret == 0) ? Character::LAST_CLASS - 1 : ret - 1;
        else if (c == KEY_DOWN)
            ret = (ret == Character::LAST_CLASS - 1) ? 0 : ret + 1;

        //Redraw pointers
        mvwprintw(w, 3 + ret*2, 2, left_ptr.c_str());
        mvwprintw(w, 3 + ret*2, pos.w - right_ptr.length() - 2, right_ptr.c_str());
        wrefresh(w);
    } while ((c = getch()) != '\n');

    //Cleanup
    delwin(w);
    return (Character::Class) ret;
}

std::string
GUI::GetString(std::string prompt) {
    return "Johnson";
}


void
GUI::ShowSplash(void) {
    GUI::ShowSplash(COL_RED);
}

void
GUI::ShowSplash(Color col) {
    WINDOW *w;
    Rect pos;

    erase();
    refresh();

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    wattron(w, COLOR_PAIR(col));
    mvwprintw(w, 0, 0, SplashStr);

    wattroff(w, COLOR_PAIR(col));
    wrefresh(w);
    refresh();
    delwin(w);
}

void
GUI::StartScreen(void) {
    GUI::ShowSplash(COL_RED);
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
