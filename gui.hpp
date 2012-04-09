#ifndef GUI_H
#define GUI_H

#include <string>

class GUI {
    public:
    void SetStatus(std::string);
    void RedrawStatus(void);

    private:
    static std::string status_line;
};

#endif
