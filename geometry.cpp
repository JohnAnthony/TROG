#include "geometry.hpp"
#include <ncurses.h>

#define ABS(x)  ((x < 0) ? -x : x)

// extern std::string status_line;
// 
bool
Point::operator==(Point const& other) {
    return this->x == other.x && this->y == other.y;
}

void
ScreenNoise(void) {
    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            mvaddch(e, i, '#'); //Magic number here is bad but efficient
        }
    }
}

void
Alert(std::string str) {
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

    pos.x = (pos.w - str.length()) / 2;
    mvwprintw(w, 2, pos.x, str.c_str());
    pos.x = (pos.w - press_msg.length()) / 2;
    mvwprintw(w, 4, pos.x, press_msg.c_str());
    wrefresh(w);

    while((c = getch()) != ' ');

    delwin(w);
}

bool
BinaryChoice(std::string str, char a, char b) {
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

float
CalculateDistance(Point p1, Point p2) {
    Point p3;
    float ret;

    p3.x = ABS(p1.x - p2.x);
    p3.y = ABS(p1.y - p2.y);

    ret = MAX(p3.x, p3.y) * 1.41;
    ret += MIN(p3.x, p3.y);

    return ret;
}

