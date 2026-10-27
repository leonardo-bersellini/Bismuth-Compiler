#include "conversions.h"

#include <stdint.h>

// int32 -> char[]
size_t bsm_int32_to_str(int32_t value, char* out, size_t outCapacity)
{
    //caso negativo, conversione a -unsigned int 
    bool negative = value < 0;
    uint32_t uval = negative
        ? (static_cast<uint32_t>(-(value + 1)) + 1)  // gestione -INT_MIN
        : static_cast<uint32_t>(value);

    
    char tmp[10];   // un uint32_t ha al massimo 10 cifre decimali
    int digits = 0;

	// scrittura dei valori in ordine inverso
    do {
        tmp[digits++] = '0' + static_cast<char>(uval % 10);
        uval /= 10;
    } while (uval != 0);

    // scrittura nel buffer di output, i valori vanno riordinati
    size_t len = 0;

    if (negative && len < outCapacity)
        out[len++] = '-';

    while (digits > 0 && len < outCapacity)
        out[len++] = tmp[--digits];

    return len;
}

// double -> char[]
// precisione fissa decimale: 6 cifre (troncate)
size_t bsm_double_to_str(double value, char* out, size_t outCapacity)
{
    constexpr int PRECISION = 6;

    //gestione valore negativo
    bool negative = value < 0.0;
    double absValue = negative ? -value : value;

    //divisone di parte intera e decimale
    uint64_t intPart = static_cast<uint64_t>(absValue);
    double fracPart = absValue - static_cast<double>(intPart);

    //spostamento della prte decimale a sinistra della virgola
    uint64_t scale = 1;
    for (int i = 0; i < PRECISION; ++i) scale *= 10;   // scale = 10^PRECISION

    uint64_t fracDigits = static_cast<uint64_t>(fracPart * static_cast<double>(scale));

    //scrittura del numero nel buffer
    size_t len = 0;
    if (negative && len < outCapacity)
        out[len++] = '-';

    char tmp[20];
    int digits = 0;
    uint64_t v = intPart;
    do {
        tmp[digits++] = '0' + static_cast<char>(v % 10);
        v /= 10;
    } while (v != 0);

    while (digits > 0 && len < outCapacity)
        out[len++] = tmp[--digits];

    //inserimento del punto decimale
    if (len < outCapacity)
        out[len++] = '.';

    //buffer per cifre decimali, riempito con '0' per valori mancanti
    char fracTmp[PRECISION];
    for (int i = PRECISION - 1; i >= 0; --i) {
        fracTmp[i] = '0' + static_cast<char>(fracDigits % 10);
        fracDigits /= 10;
    }

    for (int i = 0; i < PRECISION && len < outCapacity; ++i)
        out[len++] = fracTmp[i];

    return len;
}

// bool -> char[]
const char* bsm_bool_to_str(bool value, size_t& len) 
{
    const char* str = value ? "true" : "false";
    len = value ? 4 : 5;
    return str;
}

// char[] -> int32
int32_t bsm_str_to_int32(char* str, size_t str_size, bool& ok)
{
    ok = true;

    auto err = [&ok](int code) -> int32_t {
        ok = false;
        return code;
    };

    if(str == nullptr || str_size == 0)
        return err(-1);

    size_t i = 0;
    bool negative = false;

    // controllo segno
    if(str[i] == '-') {
        negative = true;
        ++i;
    } 
    else if(str[i] == '+') ++i;

    //deve esserci almeno una cifra
    if(i >= str_size)
        return err(-2);

    // limite assoluto consentito, gestito senza overflow
    const uint32_t limit = negative
        ? 2147483648u
        : 2147483647u;

    uint32_t value = 0;

    for (; i <= (str_size-1); ++i) {
        char c = str[i];

        // carattere non numerico
        if (c < '0' || c > '9')
            return err(-3);

        uint32_t digit = static_cast<uint32_t>(c - '0');

        // controllo overflow prima della moltiplicazione
        if (value > limit / 10 || (value == limit / 10 && digit > limit % 10))
            return err(-4);

        value = value * 10 + digit;
    }

    if (negative) {
        if (value == 2147483648u)
            return err(-5);

        return -static_cast<int32_t>(value);
    }

    return static_cast<int32_t>(value);
}

#include <stdio.h>
// char[] -> bool
bool bsm_str_to_bool(char* str, size_t str_size, bool& ok)
{
    ok = true;
    
    auto err = [&ok]() -> bool {
        ok = false;
        return false;
    };

    if(str == nullptr || str_size == 0)
        return err();

    //true
    if(str_size == 4 && str[0] == 't' 
        && str[1] == 'r' && str[2] == 'u' && str[3] == 'e') return true;
    if(str_size == 1 && str[0] == '1') return true;

    //false
    if(str_size == 5 && str[0] == 'f' && str[1] == 'a' 
        && str[2] == 'l' && str[3] == 's' && str[4]=='e') return false;
    if(str_size == 1 && str[0] == '0') return false;

    return err();
}

// char[] -> char
char bsm_str_to_char(char* str, size_t str_size, bool& ok)
{
    ok = true;
    if(str == nullptr || str_size == 0) {
        ok = false;
        return ' ';
    }
    return str[0];
}

