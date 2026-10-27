#include "input.h"

#include <windows.h>

//funzione di utility inline
//determina la dimensione effettiva di una stringa letta in input, utilizzando le 
//escape sequences di invio di windows. quest'ultime sono eliminate dalla lunghezza della stringa.
inline size_t get_inputed_str_size(const char* str)
{
    size_t size = 0;
    for(int i=0; str[i]!='\0'; ++i)
    {
        if(str[i] == '\r' && str[i+1] == '\n') break;
        size++;
    }
    return size;
}

//buffer: array di char passato dal chiamante (es. char[256])
//buffer_size: dimensione del buffer (es. sizeof(buff))
bool bsm_get_input(char* buffer, const size_t buffer_size)
{
    HANDLE Handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD bytesRead = 0;

    if(buffer == NULL || buffer_size == 0)
        return false;

    bool return_good = ReadConsoleA(Handle, buffer, buffer_size -1, &bytesRead, NULL); 

    if(!return_good) {
        return false;
    }

    buffer[bytesRead] = '\0';
    
    return true;
}

void bsm_get_input_int32(int32_t& target)
{
    char buff[256];
    if(bsm_get_input(buff, sizeof(buff)))
    {
        bool ok;
        target = bsm_str_to_int32(buff, get_inputed_str_size(buff), ok);

        return;
    }
    return;
}

void bsm_get_input_bool(bool& target)
{
    char buff[256];
    if(bsm_get_input(buff, sizeof(buff)))
    {
        bool ok;
        target = bsm_str_to_bool(buff, get_inputed_str_size(buff), ok);

        return;
    }
    return;
}

void bsm_get_input_char(char& target)
{
    char buff[256];
    if(bsm_get_input(buff, sizeof(buff)))
    {
        bool ok;
        target = bsm_str_to_char(buff, get_inputed_str_size(buff), ok);

        return;
    }
    return;
}
