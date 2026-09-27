#ifndef QUALIFIED_NAMES_H
#define QUALIFIED_NAMES_H

#include <vector>
#include <string>

/*
 * QUALIFIED NAMES
 * Questo header contiene le definizioni delle strutture dati utilizzate per la
 * rappresenazione dei qualified names dentro al compiler.
 */


using Qualifiers = std::vector<std::string>; 

// nome qualified risolto
struct QualifiedName {
    std::string name;
    Qualifiers qualifiers;
};

#endif //QUALIFIED_NAMES_H