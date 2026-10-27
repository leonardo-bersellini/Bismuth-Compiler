#ifndef _BSM_CONVERSIONS_
#define _BSM_CONVERSIONS_

#include <stdint.h>

// MODULO RUNTIME DI CONVERSIONE
//
// Questo modulo espone delle funzioni runtime di conversione dei tipi,
// utilizzabili da altre funzioni runtime.
//

extern "C" 
{
    // CONVERSIONE TO STRING
    //
    // Le funzione accettano come parametro la stringa stessa sulla quale scrivere il valore
    // convertito, per non dover possedere la memoria utilizzata per l'output (char* puntatore).
    // Per questo motivo ritorna invece la lunghezza.
    //
    size_t bsm_int32_to_str(int32_t value, char* out, size_t outCapacity);
    size_t bsm_double_to_str(double value, char* out, size_t outCapacity);
    const char* bsm_bool_to_str(bool value, size_t& len);

    // CONVERSIONI FROM STRING
    //
    // Le varie funzioni ritornano il valore target (convertito dalla stringa),
    // e prendono come parametri la stringa da convertire, la sua dimensione, ed un flag di controllo.
    //
    int32_t bsm_str_to_int32(char* str, size_t str_size, bool& ok);
    bool bsm_str_to_bool(char* str, size_t str_size, bool& ok);
    char bsm_str_to_char(char* str, size_t str_size, bool& ok);
}

#endif // _BSM_CONVERSIONS_