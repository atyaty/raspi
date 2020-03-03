#ifndef ATGETCH_H
#define ATGETCH_H

#include <termios.h>
#include <stdio.h>

//-------------------------------------------------------------------
class atGetch {
private:
    //static
    struct termios oldterm, newterm;

public:
    atGetch(int echo);
    ~atGetch();
    int kbhint();
    char get_char();
};

#endif // ATGETCH_H
