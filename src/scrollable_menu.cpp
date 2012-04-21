#include "scrollable_menu.hpp"
#include "gui.hpp"
#include <sstream>
#include <cstring>

#define DEFAULT_HEIGHT 20
#define TOP_BUFFER     3
#define BOTTOM_BUFFER  2

ScrollableMenu::ScrollableMenu(std::string inTitle) {
    this->sz.x = this->title.length() + 8;
    this->sz.y = DEFAULT_HEIGHT;
    this->window = GUI::NewCentredWindow(sz.x, sz.y);
    this->SetTitle(inTitle);
    this->max_shown_options = sz.y - TOP_BUFFER - BOTTOM_BUFFER;
}

void
ScrollableMenu::Reset(void) {
    this->pointer = 0;
    this->scroll_offset = 0;
    this->Options.clear();
    this->Resize(this->title.length() + 8, DEFAULT_HEIGHT);
}

void
ScrollableMenu::Resize(int x, int y) {
    this->sz.x = x;
    this->sz.y = y;
    delwin(this->window);
    this->window = GUI::NewCentredWindow(this->sz.x, this->sz.y);
}

void
ScrollableMenu::AddItem(std::string str) {
    this->Options.push_back(str);

    if (this->sz.x < (int) str.length() + 10)
        sz.x = str.length() + 10;

    this->Resize(this->sz.x, this->sz.y);
}

void
ScrollableMenu::Show(void) {
    int i;
    std::string str;
    std::stringstream ss;
    static char const * const upmsg =  "<PAGE UP>";
    static char const * const upmask = "         ";
    static char const * const downmsg =  "<PAGE DOWN>";
    static char const * const downmask = "           ";

    GUI::ShowSplash();
    box(this->window, 0, 0);

    ss << ":: " << this->title << " ::";
    str = ss.str();

    mvwprintw(this->window, 0, (this->sz.x - str.length()) / 2, str.c_str());

    // Our up and down indicators
    if (this->scroll_offset != 0)
        mvwprintw(this->window, 1, this->sz.x - strlen(upmsg) - 1, upmsg);
    else
        mvwprintw(this->window, 1, this->sz.x - strlen(upmask) - 1, upmask);
    if ((int)this->Options.size() > this->scroll_offset + this->max_shown_options + 1)
        mvwprintw(this->window, this->sz.y - 2, this->sz.x - strlen(downmsg) - 1, downmsg);
    else
        mvwprintw(this->window, this->sz.y - 2, this->sz.x - strlen(downmask) - 1, downmask);

    // Loop over our options
    i = 0;
    for (std::list<std::string>::iterator it = this->Options.begin();
            it != this->Options.end(); ++it, ++i) {

        // Only show the current page of stuff
        if (i < this->scroll_offset)
            continue;
        if (i > this->max_shown_options + this->scroll_offset)
            break;

        str = *it;

        if (i == this->pointer)
            mvwprintw(this->window, 2+i - this->scroll_offset, 2, "-->  ");
        else
            mvwprintw(this->window, 2+i - this->scroll_offset, 2, "    ");

        wprintw(this->window, str.c_str());

        for (int e = str.length(); e < this->sz.x - 9; ++e)
            waddch(this->window, ' ');
    }

    wrefresh(this->window);
    refresh();
}

void
ScrollableMenu::PtrUp(void) {
    this->pointer--;
    if (this->pointer < 0)
        this->pointer = 0;
    else if (this->pointer < this->scroll_offset)
        this->SnapScroll();
    this->Show();
}

void
ScrollableMenu::PtrDown(void) {
    this->pointer++;
    if (pointer >= (int) this->Options.size())
        pointer = this->Options.size() - 1;
    else if (this->pointer > this->scroll_offset + this->max_shown_options)
        this->SnapScroll();
    this->Show();
}

void
ScrollableMenu::PageUp(void) {
    this->pointer -= this->max_shown_options;
    if (this->pointer < 0)
        this->pointer = 0;
    this->SnapScroll();
    this->Show();
}

void
ScrollableMenu::PageDown(void) {
    this->pointer += this->max_shown_options;
    if (this->pointer >= (int)this->Options.size())
        this->pointer = this->Options.size() - 1;
    this->SnapScroll();
    this->Show();
}

void
ScrollableMenu::SetTitle(std::string str) {
    this->title = str;
}

int
ScrollableMenu::Selection(void) {
    if (this->Options.size() == 0)
        return -1;
    return pointer;
}

void
ScrollableMenu::SnapScroll(void) {
    if (this->pointer < this->scroll_offset)
        this->scroll_offset = this->pointer;
    else if (this->pointer >= this->scroll_offset + this->max_shown_options)
        this->scroll_offset = this->pointer - this->max_shown_options;
}

ScrollableMenu::~ScrollableMenu(void) {
    delwin(this->window);
}
