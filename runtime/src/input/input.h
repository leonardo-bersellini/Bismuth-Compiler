#ifndef _BSM_INPUT_
#define _BSM_INPUT_

#include <stdint.h>

// FILE SORGENTE PER IL MODULO RUNTIME DI INPUT
//
// Questo modulo implementa le funzioni di input tramite API Windows.
//

#include "conversions/conversions.h"

extern "C"
{
    bool bsm_get_input(char* buffer, const size_t buffer_size);

    // BSM INPUT_X
    //
    // Funzioni di input compatibili con target diversi da stringhe.
    // Utilizzano funzioni di conversione internamente per salvare il valore nella variabile
    // con il formato corretto.
    //
    void bsm_get_input_int32(int32_t& target);
    void bsm_get_input_bool(bool& target);
    void bsm_get_input_char(char& target);
}

#endif //_BSM_INPUT_