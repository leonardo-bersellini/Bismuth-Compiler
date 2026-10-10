#include "preprocessor.h"

#include <string>
#include <vector>
#include <ranges>

/*
 * BISMUTH PREPROCESSOR
 *
 * Il preprocessore è il componente responsabile della risoluzione delle direttive (del preprocessore).
 * Queste direttive sono eseguite prima dell'analisi effettiva del file, quindi in contemporanea
 * con il processo di tokenizzazione, poichè influiscono proprio sui token letti dai moduli successivi.
 * 
 * Il preprocessore contiene dunque il componente lexer, ed è responsabile della tokenizzazione dei
 * files, orchestrando i file e l'output finale.
 */


/*
 * Punto di entrata dell'esecuzione del preprocessore.
 * Processa il file main passato al compilatore.
 * Inizializza lo stato di esecuzione e richiama il process del file, aggiungendo il token EOF
 * alla fine.
 */

std::vector<Token> PreProcessor::process(const std::string& mainFile, ErrorLog& errorLog)
{
    m_errorLog = &errorLog;

    m_fileStack.clear();

    std::vector<Token> tokens = processFile(mainFile);

    Token eof;
    eof.type = TokenType::EndOfFile;

    tokens.push_back(eof);
    return tokens;
}

/*
 * Aggiunge un percorso di inclusione alla lista interna del sourceManager.
 */

void PreProcessor::addIncludeDir(const std::string& dir)
{
    this->m_sourceManager.addIncludeDirectory(dir);
}

/*
 * Funzione di process vera e propria. Gestisce le risorse dei file con il sourcemanager iterno,
 * tokenizza i contenuti e richiama le funzioni di esecuzione delle direttive.
 * Tramite chiamate ricorsive (dalle funzioni di gestione delle direttive), il risultato
 * della funzione è l'insieme dei token di tutti i file inclusi e delle direttive risolte.
 */

std::vector<Token> PreProcessor::processFile(const std::string& path)
{
    std::vector<Token> output;

    //risoluzione del path: path assoluto tramite include dirs
    const std::string includingFile = m_fileStack.empty() ? "" : m_fileStack.back();
    const auto resolved = m_sourceManager.resolvePath(path, includingFile);
    
    if(!resolved) {
        m_errorLog->addError(std::string("could not find included file: ") + path, TextPosition(0, 0, m_fileStack.back()));
        return output;
    }

    const auto content = m_sourceManager.getFileContent(*resolved);
    if(!content) {
        m_errorLog->addError(std::string("could not access to included file: ") + *resolved, TextPosition(0, 0, m_fileStack.back()));
        return output;
    }

    //stack: segnala includes circolari
    if(std::ranges::find(m_fileStack, *resolved) != m_fileStack.end()) {
        m_errorLog->addWarning(std::string("circular include, ignoring directive for file: ") + *resolved);
        return output;
    }
    m_fileStack.push_back(*resolved);

    //tokenizzazione
    m_lexer.setSourceFile(*resolved);
    std::vector<Token> tokens = m_lexer.analiseString(*content, *m_errorLog);

    //gestione dei tokens
    for(size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& t = tokens[i];

        if(t.type == TokenType::EndOfFile) continue;

        if(t.type == TokenType::PreprocessorDirective)
            handleDirective(tokens, i, output);
        else
            output.push_back(t);
    }

    m_fileStack.pop_back();
    return output;
}

/*
 * Elabora le direttive, eseguendo il dispatch sulle funzioni di handling, che modificano il vettore
 * di ritorno finale. Come convenzione, consuma il token che indica il nome stesso della direttiva,
 * lasciando solo tokens utili per la funzione di handling.
 */

void PreProcessor::handleDirective(const std::vector<Token>& tokens, size_t& index, std::vector<Token>& output)
{
    //token corrente: direttiva
    std::string directive = tokens[index].lexeme;
    index++;

    //dispatch in base alla direttiva
    if(directive == "include") 
    {
        handleInclude(tokens, index, output);
    }
    else {
        m_errorLog->addError("unknown directive: " + directive, tokens[index-1].position);
        index--; //nessuna direttiva, riporta i token allo stato iniziale
    } 

    return;
}

/*
 * Gestisce una direttiva di include, richiamando un processo sul file in questione.
 */

void PreProcessor::handleInclude(const std::vector<Token>& tokens, size_t& index, std::vector<Token>& output)
{
    Token includeArgument = tokens[index];
    const std::string path = includeArgument.lexeme;

    const auto processed = processFile(path);
    
    output.insert(output.end(), processed.begin(), processed.end());

    return;
}
