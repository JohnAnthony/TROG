#include "gui.hpp"
#include <ncurses.h>

extern std::string GUI::status_line;

void
GUI::RedrawStatus(void) {
    mvprintw(LINES -1, 0, GUI::status_line.c_str());
    for (int i = GUI::status_line.length(); i <= COLS; ++i)
        addch(' ');
}

void
GUI::SetStatus(std::string str) {
    GUI::status_line = str;
}
