#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <vector>
#include <functional>
#include <unordered_map>

#include "core/tokens.h"
#include "sourcemanager/sourcemanager.h"
#include "lexer/lexer.h"

class PreProcessor 
{
public:
    std::vector<Token> process(const std::string& mainFile, ErrorLog& errorLog);

    void addIncludeDir(const std::string& dir);

private:
    Lexer m_lexer;
    SourceManager m_sourceManager;
    ErrorLog* m_errorLog;

    //stack dei file inclusi 
    std::vector<std::string> m_fileStack;

    std::vector<Token> processFile(const std::string& path);

    void handleDirective(const std::vector<Token>& tokens, size_t& index, std::vector<Token>& output);

    void handleInclude(const std::vector<Token>& tokens, size_t& index, std::vector<Token>& output);
    
};

#endif //PREPROCESSOR_H