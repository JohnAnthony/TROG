#ifndef SCROLLABLE_MENU_H
#define SCROLLABLE_MENU_H

#include <list>
#include <string>
#include <ncurses.h>
#include "geometry.hpp"

class ScrollableMenu {
    public:
    ScrollableMenu(std::string inTitle);
    ~ScrollableMenu(void);
    void Reset(void);
    void Resize(int x, int y);
    void Show(void);
    void PtrUp(void);
    void PtrDown(void);
    void PageUp(void);
    void PageDown(void);
    void AddItem(std::string str);
    void SetTitle(std::string str);
    int Selection(void);
    void SnapScroll(void);

    WINDOW *window;
    int pointer;
    int scroll_offset;
    std::list<std::string> Options;
    std::string title;
    Point sz;
    int max_size;
    int max_shown_options;
};

#endif
