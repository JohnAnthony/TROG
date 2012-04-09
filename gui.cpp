#include "gui.hpp"
#include <ncurses.h>

#define NOISE_CHARACTER  '#'

std::string GUI::status_line;
std::list<std::string> GUI::messages;
const char *GUI::SplashStr = 
"\n\
\n\
\n\
\n\
\n\
.ddhddddmdddhhdddddm/                          ....              -oydNMMMNNmd:  \
:No:...`/MMM:.````.ysodNNdyyyhddy/       ./sddhyyydmmy+.        sMNs:.``.-omM:  \
+       -MMm``     ` `+MMy``  `oNh+.   .yMNo-```   `:hMMh:    :NMh.``      .m.  \
        `mNM.         :MMs`     yMMs  .mMN:``         /NMM+  -NMm.`         s`  \
        `hdM.         /MMs`     sMM+  hMMo`            +MMM: sMMs`              \
        .MMN.         /MMy`    :mNo   MMM+`             NMMs`mMM+`              \
        `MMN.         /MMdo++odMm-    MMMd`             mMMs`NMM+`    `:::/++   \
        .MMN`         /MMy-...oddh-   oMMM+            -MMN-`mMMs`     .-dMMm   \
        /MMM.         /MMy`    .yMN+   +NMNo          -mMd-` /MMm.       :MMy   \
        -MMM.         sMMh`     `oNMh:  .omMmo-....:+hmh+.`   hMMh`      -MMs   \
        +MMM`       :+ssso+:      .sNMh/- `-/ossyyyso/.``     `sMMy.     -MMs   \
        hMMM/                       `/+ss+.     ```             -hNNy+:-`.dMy   \
     .:oyyyhh/.                                                   ./oyyyys+:.   \
            ```                                                        ``       \
\n\
\n\
\n\
\n\
\n";

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
    return Character::ORC;
}

Character::Class
GUI::SelectClass(void) {
    return Character::BARBARIAN;
}

std::string
GUI::GetString(std::string prompt) {
    return "Johnson";
}

void
GUI::ShowSplash(void) {
    WINDOW *w;
    Rect pos;

    erase();
    refresh();

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    mvwprintw(w, 0, 0, SplashStr);
    mvwprintw(w, 18, 30, "Press any key to begin...");

    wrefresh(w);
    getch();
    delwin(w);
}

void
GUI::ScreenNoise(void) {
    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            mvaddch(e, i, '#'); //Magic number here is bad but efficient
        }
    }
}

