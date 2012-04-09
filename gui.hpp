#ifndef GUI_H
#define GUI_H

#include "game.hpp"
#include <string>
#include <list>

class GUI {
    public:
    void SetStatus(std::string);
    void RedrawStatus(void);
    void AddMessage(std::string str);
    void ProcessMessages(Game *g);

    private:
    static std::string status_line;
    static std::list<std::string> messages;
};

#endif
