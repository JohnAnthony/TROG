#include "gui.hpp"
#include <ncurses.h>

std::string GUI::status_line;
std::list<std::string> GUI::messages;
const char *GUI::SplashStr = 
"ddddddhhhhhhhhhhhhhhhhhhhhdddddddddddddddddNNNNNmmmmmdmdhhyhdhyhhyyysyhsysoyhsoo\
ddddhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhdmNNNNddmmmmdNmyhmmddhdhddhddddhsyyoso\
dhhhhhhhhhhhhhhhhhhhhhhhhhhhhhyssyyhhyddddNNmNNNmmdyhyhyyo+yhyoshyhhhdhhhhshyyhy\
hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhyyhhyhddddmNNNNmNNmmmhyshhhyyyyssyhyhhhhyhhdhdddhy\
hhhhhhhhhhhhhhhhhhhhyyyyyhhhhyhhhhdddddddmmmmmmmmmmdhyysooo+osydddhhyhyohhymmmdh\
hhhhhhhhhhhhhhhhhhyhysyyyhhysossyhdddddddddmmmmmmmmmmddhyssssyhddhdddmyyddhmmmmN\
hhhhhhhhhhyhhyyyhhhysshddhyssssyhhdddddddddmmmmmNNNmNmmmddhhhdhdysyhdmoyhddmNNMM\
hhhhhhhyyyhhysyyhhhyyhdddhhhyyysyhhdddddddddmmmNNMMNNNmmmdddhhhdsosydmhhddmNNMMM\
hyyhyyyyyydyyhdddhhhdddddhhhhhyyyyyyhhhddddddmmmNNNmmmddhsoohhhhhyyyshmmdmNMMMMN\
hyhhhdyyhddddhyyyyydddddmdddhhyyysyyhhhdddddddddddddddhy:::oyhhhhyhyhhdmNNNNNNNN\
yyddddddmhdddyyyhhdddmmmmmmdhyyyyyyhddddddmmddddddddyys/.:+ss++yhddhyyhhhdmmmmmm\
sshhddhyhyhddhhhhdddmmmmmmddddhhhhhddddddddddddddhhyoso-/o+/oo++yhyyyyyyyydmmdhh\
ssyyhhyyhydddhhhddddmNNMNNdhhhhyyhhhdddmmddmddddddyyss+/+//+ysooosyhhhyyysyhyyhd\
yyyyyyyyyhddhyhddddmNMMMNMNmdddhhhhyhhdmmmmmdhddddhhyo+o+/+yyooooyyyyhhyhyyyyhhh\
yyyyyhhyhdhhyhdddmmNMMMMNNmmmdmmddhhhhhdmddhhhhhhhhysoo+/+shdyyssssyhyyhhyyhhhhh\
hhyyyyyhhyhhdddmmNNMMMMNmhdddddddddhhhyyyhhhhhhhyyso+o+/:oyyddhyyssyyyyyysyyyyys\
hhyyyyyyhhyyddmmNMMMMNmdyhdddddddhyyysssyyyyyhyyooo+++//+syhddhyhyssyyyyyyyyyhyy\
hhhhyhyhhsssdmmNNMMNNmhdyhhdddhhyyyyyyyyhhyyysssooo+++//oyyhdddhhysssssosyyssssy\
yyhyyyhhysyydNmNMMNhhhhhyyhhhyhhhyyhyyyhhhyysssssssooo+oyyhdmmmdhyyyyhsooosssyyy\
ssssyddhssyhhmNMMMhhhhhysshdhhhhyhyhyyyhhhhysssyhyyoooyhdmdmmNmddhyhhddhyssssyhy\
sssshhhyyyyyhmMMNhhyyhhhhhdhddddhyyyyyyhhhhhssyyhhhsohddhdmNNNmhddhyyyddysssssyy\
dyyhdhhyhyhddNNmhyyyhhdddhhhhhhhhhyyyysyddhhyyyyyhhysdhhhhhmNNNhyyhhysyyysyyysss\
yyhhhhyhddddmmhysssyhddmmmmmmmmddhyyyyyyyhhyyyssyyyoohhhhyyhNMNNdhyyyyssssyyhyys\
syhhyyyyhhddhyyshhyyhdddddmmmmmmmmmmddddddhyyyyysso+/ohhyshddNMNNmdyssyyyyyyhs++\
shhysssyhdhyyysyyhhhdhhyyhdhhhhdddmddddmmmddhyyssoo+//oyhhhmmmNMNNmdysyyshhhyyhh";

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

    pos.w = 80;
    pos.h = 25;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);
    mvwprintw(w, 0, 0, SplashStr);

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

