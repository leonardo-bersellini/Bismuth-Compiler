#ifndef _BSM_PRINT_
#define _BSM_PRINT_

#include <stdint.h>

#ifdef __linux__
    #include <cstddef>
#endif

// FILE SORGENTE PER IL MODULO RUNTIME DI PRINT
//
// Questo modulo implementa la funzione di print tramite API Windows. (consoleApi).
// La funzione è dichiarata come extern C e non è dunque legata al linguaggio c++.
//

#include "conversions/conversions.cpp"

extern "C" 
{
    bool bsm_print(const char* data, size_t len);

    // BSM_PRINT_X
    //
    // Queste funzioni stampano valori che non sono stringhe, richiamando internamente
    // funzioni runtime di conversione e lo stesso bsm_print.
    //
    // Contratto delle funzioni:
    // Le seguenti sono richiamate dal modulo di codegen come funzioni linkate a runtime,
    // devono quindi rispettare il contratto: <void> func_identifier(<T>) dove T è il tipo che va
    // automaticamente convertito in stringa da stampare.
    //
    void bsm_print_int(int32_t integer);
    void bsm_print_double(double value);
    void bsm_print_char(char value);
    void bsm_print_bool(bool value);
}

#endif //_BSM_PRINT_