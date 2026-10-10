#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <string>

#include "namespace_table.h"
#include "core/symbols.h"

// Tabella di gestione dei namespace
using NamespaceTable = namespace_table<std::string, Symbol>;

#endif //NAMESPACE_H