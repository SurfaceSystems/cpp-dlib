#include "getch.hpp"

#ifdef _WIN32

#include <conio.h>

int getch()
{
    return _getch();
}

int _getch()
{
    return ::_getch();
}

#else

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

// Read a character without waiting for Enter 
int getch()
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO,
              TCSANOW,
              &newt);

    int ch = getchar();

    tcsetattr(STDIN_FILENO,
              TCSANOW,
              &oldt);

    return ch;
}

// Windows-like alias 
 int _getch()
{
    return getch();
}

#endif
