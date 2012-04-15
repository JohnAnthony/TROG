#include "debug.hpp"
#include "gui.hpp"

void
Debug::DebugConsole(Game *g) {
    std::string s;
    int i;

    s = GUI::GetString("Debug Command:");

    //Basic commands with no arguments
    if (s == "LevelUp") {
        i = g->character->next_level - g->character->getXP();
        g->character->GiveXP(i);
    }

    GUI::DoRedraw();
}
