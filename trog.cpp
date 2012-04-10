#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include <signal.h>

Game *g = NULL;

static void
resize_handler(int sign) {
    g->HandleResize(sign);
}

static void
interrupt_handler(int sign) {
    erase();
    endwin();
    puts("SIGINT received; exiting cleanly.");
    exit(0);
}

int main(int argc, char** argv) {
    bool playagain;
    Character *c;

    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    signal(SIGINT, interrupt_handler);

    for (int i = 0; i < 8; ++i) 
        init_pair(i, i, COLOR_BLACK);

    do {
        erase();
        GUI::StartScreen();
        c = GUI::CharacterCreation();
        g = new Game(c);
        playagain = g->Run();
        delete g;
        delete c;
    } while (playagain);

    endwin();

    return 0;
}
