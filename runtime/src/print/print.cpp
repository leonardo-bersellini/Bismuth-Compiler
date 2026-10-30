#include "print.h"

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <cstdio>
#endif

// data: caratteri da stampare in stdout
// len: lunghezza del buffer
bool bsm_print(const char* data, size_t len)
{
    #ifdef _WIN32
        HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written = 0;
        bool good = WriteConsoleA(Handle, data, len, &written, NULL);
    
    #elif defined(__linux__)
        //argomenti: buffer, dimensione di ogni elemento in byte, lunghezza
        bool good = fwrite(data, 1, len, stdout);
    #endif

    if(!good) {
        return false;
    }

    return true;
}

void bsm_print_int(int32_t integer)
{
    char buffer[64];
    size_t size = bsm_int32_to_str(integer, buffer, sizeof(buffer));

    bsm_print(buffer, size);
}

void bsm_print_double(double value)
{
    char buffer[64];
    size_t size = bsm_double_to_str(value, buffer, sizeof(buffer));

    bsm_print(buffer, size);
}

void bsm_print_char(char value)
{
    bsm_print(&value, 1);
}

void bsm_print_bool(bool value)
{
    size_t s;
    const char* str = bsm_bool_to_str(value, s);

    bsm_print(str, s);
}

