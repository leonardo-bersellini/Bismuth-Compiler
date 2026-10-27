#include "print.h"

#include <windows.h>

// data: caratteri da stampare in stdout
// len: lunghezza del buffer
bool bsm_print(const char* data, size_t len)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD written = 0;
    BOOL return_good = WriteConsoleA(Handle, data, len, &written, NULL);

    if(!return_good) {
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

