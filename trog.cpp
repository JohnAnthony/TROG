#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "geometry.hpp"

int main(int argc, char** argv) {
    Game *g;

    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    raw();
    curs_set(0);
    start_color();

    init_pair(CustColour::WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(CustColour::RED, COLOR_RED, COLOR_BLACK);
    init_pair(CustColour::YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CustColour::GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(CustColour::BLUE, COLOR_BLUE, COLOR_BLACK);

    g = new Game();
    g->Run();
    delete g;
    endwin();

    return 0;
}
