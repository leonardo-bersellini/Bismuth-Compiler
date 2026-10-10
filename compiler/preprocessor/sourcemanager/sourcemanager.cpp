#include "sourcemanager.h"

#include <filesystem>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;


/*
 * Aggiunge un path alla lista di includeDirs, controllando prima se il path esiste ed è una
 * directory valida. non ritorna errori.
 */

void SourceManager::addIncludeDirectory(const std::string& path)
{
    fs::path dir(path);

    if(!fs::exists(dir)) return;
    if(!fs::is_directory(dir)) return;

    m_includeDirs.push_back(path);
}

/*
 * Legge il contenuto di un file. 
 * Ritorna un valore nullopt se il file passato non è valido oppure non è leggibile.
 */

std::optional<std::string> SourceManager::getFileContent(const fs::path& path)
{
    std::ifstream in(path);

    if(!in) return std::nullopt;

    std::stringstream buffer;
    buffer << in.rdbuf();

    return buffer.str();
}

/*
 * Risolve il percorso scritto in una direttiva di include, trasformandolo nel percorso assoluto
 * e normalizzato di un file esistente. Ritorna un valore nullopt in caso di errore.
 *
 * Ordine di ricerca per i percorsi relativi:
 *   1. la cartella del file che contiene l'include (includingFile), se specificato
 *   2. le include directories del SourceManager, nell'ordine in cui sono state aggiunte
 * Un percorso già assoluto viene verificato direttamente.
 */

std::optional<std::string> SourceManager::resolvePath(const std::string& path,
                                                      const std::string& includingFile)
{
    const fs::path requested(path);
    std::vector<fs::path> candidates;

    const fs::path baseDir = fs::path(includingFile).parent_path();

    if(requested.is_absolute()) {
        candidates.push_back(requested);
    } else {
        //risultato relativo al file chiamante
        candidates.push_back(baseDir / requested);

        //aggiunta delle include dirs
        for(const auto& dir : m_includeDirs)
            candidates.push_back(fs::path(dir) / requested); 
    }

    //cerca un candidato valido (file esistente)
    for(const auto& candidate : candidates)
    {
        std::error_code ec;

        if(!fs::is_regular_file(candidate, ec) || ec) continue;

        const fs::path canonical = fs::canonical(candidate, ec);
        if(ec) continue;

        return canonical.string();
    }

    return std::nullopt;
}