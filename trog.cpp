#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "game.hpp"
#include "character.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include <signal.h>
#include <iostream>

Game *g = NULL;
Character *c;

static void
resize_handler(int sign) {
    if (g)
        g->HandleResize(sign);
    else
        refresh();
}

static void
interrupt_handler(int sign) {
    erase();
    endwin();
    puts("SIGINT received; exiting cleanly.");
    delete g;
    delete c;
    exit(0);
}

int main(int argc, char** argv) {
    bool playagain;

    srand(time(NULL));
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    if (COLS < 80 || LINES < 25) {
        endwin();
        std::cout << "ERROR: This game requires a terminal size of at least 80x25\n";
        exit(1);
    }

    signal(SIGINT, interrupt_handler);
    signal(SIGWINCH, resize_handler);

    for (int i = 0; i < 8; ++i) 
        init_pair(i, i, COLOR_BLACK);

    do {
        erase();
        GUI::StartScreen();
        c = GUI::CharacterCreation();
        g = new Game(c);
        playagain = g->Run();
    } while (playagain);

    endwin();
    delete g;
    delete c;

    return 0;
}
