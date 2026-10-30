#include "ansi.h"

#ifdef _WIN32

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    void ansi::enableAnsi() 
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);

        ansi_enabled = true;
    }

#elif defined(__linux__)
    void ansi::enableAnsi()
    {
        ansi_enabled = true;
    }

#endif

void ansi::disableAnsi()
{
    ansi_enabled = false;
}