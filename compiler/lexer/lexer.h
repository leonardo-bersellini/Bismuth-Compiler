#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <optional>

#include "token.h"
#include "errors/errorlog.h"

class Lexer
{
public:
    Lexer();

    void setSourceFile(const std::string& sourceFile);
    std::vector<Token> analiseString(const std::string& string, ErrorLog& _errorLog);

private:
    std::vector<Token> m_tokens;
    std::string buffer;
    int indexPos;
    TextPosition currentTextPos;
    ErrorLog* errorLog;

    char peek(int offset = 0) const;  
    char advance(); 

    bool isAtEnd() const;
    bool isAtEnd(int pos) const;

    bool isDigit(const char& c) const;
    bool isAlpha(const char& c) const;

    Token createToken(TokenType type);

    void skipIgnored();
    std::optional<Token> scanOperator();
    
    Token scanDirective();

    Token scanNumber();
    Token scanIdentifier();
    Token scanString();
    Token scanChar();

};

#endif // LEXER_H
