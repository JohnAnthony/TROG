#include "debug.hpp"
#include "gui.hpp"
#include <iostream>
#include <sstream>

void
Debug::DebugConsole(Game *g) {
    std::string s;
    std::string cmd;
    int arg;

    arg = 0;
    s = GUI::GetString("Debug Command:");
    std::stringstream iss(s);
    iss >> cmd;
    if (iss)
        iss >> arg;

    if (arg == 0)
        arg = 1;

    for (; arg > 0; --arg) {
        if (cmd == "LevelUp")
            g->character->GrantLevel();
        else if (cmd == "GoDown")
            g->GoDownALevel();
        else if (cmd == "GoUp")
            g->GoUpALevel();
        else if (cmd == "AllSpells")
            g->character->GiveAllSpells();
        else if (cmd == "RevealAll") {
            g->cur_level->RevealAll();
            break;
        }
        else if (cmd == "ClearObstacles") {
            g->cur_level->ClearObstacles();
            break;
        }
        else {
            GUI::Alert("Unknown debug command.");
            break;
        }
    }

    GUI::DoRedraw();
}
