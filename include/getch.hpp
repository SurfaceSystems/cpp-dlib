// getch.hpp - Change this
#ifndef GETCH_HPP
#define GETCH_HPP

#ifdef _WIN32
    // For Windows, use the Windows API directly instead of getch
    #include <conio.h>
    // Don't redeclare getch - use the one from conio.h
#else
    // Unix implementation
    #include <termios.h>
    #include <unistd.h>
    int getch();
#endif

#endif // GETCH_HPP