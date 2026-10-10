#include "lexer.h"

#include <iostream>
#include <cctype>
#include <climits>
#include <string>
#include <vector>
#include <optional>

#include "lexemes.h"
#include "core/keywords.h"
#include "utils/ansi/ansi.h"

Lexer::Lexer() {}

/*
 * Queste due versioni della stessa funzione controllano se il buffer è terminato, in base alla
 * posizione dell'indice oppure ad una assegnata
 */

bool Lexer::isAtEnd() const {
    return (indexPos >= buffer.size());
}

bool Lexer::isAtEnd(int pos) const {
    return (pos >= buffer.size());
}

/*
 * Queste due funzioni sono utiliti necessarie per rendere il codice leggibile.
 * Si occupano del controllo con cctype di un carattere.
 */

bool Lexer::isDigit(const char& c) const {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isAlpha(const char& c) const {
    return c == '_' || std::isalpha(static_cast<unsigned char>(c));
}

/*
 * Analizza i caratteri futuri nel buffer che sta vanendo analizzato, scorrendo di un numero
 * assegnato di posizioni.
 */

char Lexer::peek(int offset) const
{
    int position = indexPos + offset;
    if(isAtEnd(position)) return char('\0');
    return buffer.at(position);
}

/*
 * Consuma il carattere corrente, aggiornando il buffer, 
 * l'indice e la posizione espressa in righe-colonne.
 */

char Lexer::advance() {
    if(isAtEnd()) return '\0';

    char r = buffer.at(indexPos);

    if(r == '\n') {
        currentTextPos.line++;
        currentTextPos.column = 1;
    }
    else currentTextPos.column++;

    indexPos++;
    return r;
}

/*
 * Genera un token basandosi su un tipo dato
 */

Token Lexer::createToken(TokenType type) {
    Token t;
    t.type = type;
    t.position = currentTextPos;
    t.lexeme = advance();
    return t;
}


/*
 * Helper per impostare il valore del file sorgente corrente.
 */

void Lexer::setSourceFile(const std::string& sourceFile)
{
    currentTextPos.source_file = sourceFile;
}

/*
 * Punto di entrata dell'analisi lessicale.
 * Questa funzione analizza una stringa assegnata dividendola in tokens secondo la grammatica del
 * linguaggio. Per dividere i caratteri in token, analizza i singoli caratteri per richiamare funzioni
 * di scan ed estrapolare tutti i caratteri che andranno a formare i tokens.
 */

std::vector<Token> Lexer::analiseString(const std::string &string, ErrorLog &_errorLog)
{
    this->errorLog = &_errorLog;
    buffer = string;

    m_tokens.clear();
    indexPos = 0;
    currentTextPos.column = 1;
    currentTextPos.line = 1;

    while(!isAtEnd())
    {
        skipIgnored();
        if(isAtEnd()) break;

        char c = peek();

        if(isDigit(c))       m_tokens.push_back(scanNumber());
        else if(isAlpha(c))  m_tokens.push_back(scanIdentifier());
        else if(c == '"')    m_tokens.push_back(scanString());
        else if(c == '\'')   m_tokens.push_back(scanChar());
        else if(auto op = scanOperator()) m_tokens.push_back(*op);
        else if(c == '#')    m_tokens.push_back(scanDirective());
        else
        {
            m_tokens.push_back(createToken(TokenType::Unknown));
            errorLog->addError(std::string("carattere non riconosciuto. char: ") + c, currentTextPos);
        }
    }

    Token eof;
    eof.type = TokenType::EndOfFile;
    eof.position = currentTextPos;
    m_tokens.push_back(eof);

    return m_tokens;
}

/*
 * Questa funzione gestisce la lettura di tutti i caratteri ignorati, ovvero caratteri nascosti
 * da commenti oppure sequenze speciali non riconosciute come tokens.
 */

void Lexer::skipIgnored()
{
    while(!isAtEnd())
    {
        char c = peek();

        if(c == ' ' || c == '\n' || c == '\t') {
            advance();
        }
        else if(c == '/' && peek(1) == '/') {
            while(!isAtEnd() && peek() != '\n') advance();
        }
        else if(c == '/' && peek(1) == '*') {
            advance(); // '/'
            advance(); // '*'

            while(!isAtEnd() && !(peek() == '*' && peek(1) == '/')) advance();

            if(isAtEnd()) {
                errorLog->addError("Unterminated multi-line comment", currentTextPos);
            } else {
                advance(); // '*'
                advance(); // '/'
            }
        }
        else break;
    }
}

/*
 * Si occupa di individuare gli operators lessicali, ovvero quei lexemes fissi che rappresentano
 * di per sè un token. Lo sono tutti i lexemes della tabella operatorsTable.
 * Questa funzione utilizza un approccio detto "maximal munch", ovvero considera corretto il token
 * formato dal maggior numero possibile di caratteri.
 * */

std::optional<Token> Lexer::scanOperator()
{
    for(std::size_t len = getMaxOperatorLen(); len > 0; --len)
    {
        std::string candidate;
        for(std::size_t i = 0; i < len; ++i)
            candidate.push_back(peek(static_cast<int>(i)));

        auto it = operatorsTable.find(candidate);
        if(it == operatorsTable.end()) continue;

        Token t;
        t.type = it->second;
        t.position = currentTextPos;
        for(std::size_t i = 0; i < len; ++i)
            t.lexeme.push_back(advance());
        return t;
    }
    return std::nullopt;
}

/*
 * Questa funzione esegue l'analisi di una direttiva del preprocessore, ignorandone il contenuto.
 * La direttiva corrisponde ad un identifier, preceduta da un token speciale.
 */

Token Lexer::scanDirective()
{
    advance(); //consuma '#'

    Token directive = scanIdentifier();

    directive.type = TokenType::PreprocessorDirective;
    return directive;
}


/*
 * Questa funzione esegue l'analisi dei caratteri numerici, delimitando dei token
 * che corrispondono a numeri letterali.
 */

Token Lexer::scanNumber()
{
    Token token;
    token.position = currentTextPos;

    std::string number; //numero contenuto al termine dello scan

    // Lettura delle cifre
    while((!isAtEnd()) && isDigit(peek())) {
        number.push_back(advance());
    }

    // Controllo isDouble
    if((peek() == '.') && isDigit(peek(1))) {
        //consuma il punto
        number.push_back(advance());

        // Lettura dei decimali
        while((!isAtEnd()) && isDigit(peek())) {
            number.push_back(advance());
        }

        // Numero double
        token.type = TokenType::DoubleLiteral;
        token.numericValue = std::stod(number);
        token.lexeme = number;

        return token;
    }

    // Numero integer
    token.type = TokenType::IntegerLiteral;
    try {
        token.numericValue = std::stoi(number);
    } 
    catch (std::out_of_range) {
        errorLog->addError("numeric value specified exceed range limits (num:" + number + ") " +
                        "(limit: " + std::to_string(INT_MAX) + ")", currentTextPos);
    }
    token.lexeme = number;

    return token;
}

/*
 * Questa funzione analizza i caratteri a partire da lettere, per delimitare degli identificatori.
 * Una volta delimitato un identificatore, controlla anche se corrisponde ad una keyword, per
 * classificarla come tale.
 */

Token Lexer::scanIdentifier() {
    Token token;
    token.position = currentTextPos;

    std::string identifier; //testo dell'identificatore

    //controllo lettere
    while(!isAtEnd() && (isDigit(peek()) || isAlpha(peek()))) {
        char l = advance();
        identifier.push_back(l);
    }

    //check for keyword
    if(keywords.contains(identifier)) {
        token.type = keywords.at(identifier); //tipo corrispondente alla keyword
        token.lexeme = identifier;
        return token;
    }

    token.type = TokenType::Identifier;
    token.lexeme = identifier;

    return token;
}

/*
 * Esegue l'analisi dei caratteri per indentificare una stringa letterale
 */

Token Lexer::scanString() {

    std::string stringa;
    TextPosition pos = currentTextPos;

    advance(); //consuma "

    while(!isAtEnd() && (peek() != '"')) {

        //escape sequence
        if(peek() == '\\') {
            advance(); //consuma escape

            if (isAtEnd()) {
                // backslash a fine input, senza carattere successivo
                errorLog->addError("invalid escape sequence at end of input", currentTextPos);
                Token errorTok;
                errorTok.type = TokenType::Unknown;
                errorTok.position = pos;
                return errorTok;
            }

            char escaped = advance();

            if (escaped == 'n') stringa.push_back('\n');
            else if (escaped == '"') stringa.push_back('"');
            else if (escaped == '\\') stringa.push_back('\\');
            else {
                errorLog->addError("invalid escape sequence: \\" + std::string(1, escaped), currentTextPos);
                Token errorTok;
                errorTok.type = TokenType::Unknown;
                errorTok.position = pos;
                return errorTok;
            }
        }
        else  stringa.push_back(advance());
    }

    Token returnTok;
    returnTok.position = pos;
    returnTok.lexeme = stringa;
    returnTok.type = TokenType::StringLiteral;

    if(isAtEnd()) {
        //stringa non terminata
        errorLog->addError("missing terminating \" character", currentTextPos);
        returnTok.type = TokenType::Unknown;
    }
    else advance(); //consuma " terminatore

    return returnTok;
}

/*
 * Esegue l'analisi dei caratteri per indentificare un char letterale
 */

Token Lexer::scanChar() {
    TextPosition pos = currentTextPos;
    advance(); //consuma '

    if(isAtEnd() || peek() == '\'') {
        // char non chiuso o vuoto
        errorLog->addError("empty or invalid char literal", pos);
        Token errorTok;
        errorTok.type = TokenType::Unknown;
        errorTok.position = pos;
        return errorTok;
    }

    char ch = advance(); //char dentro agli apici

    if (isAtEnd() || peek() != '\'') {
        // char non chiuso o multi-char
        errorLog->addError("char literal must contain exactly one character", pos);
        Token errorTok;
        errorTok.type = TokenType::Unknown;
        errorTok.position = pos;
        return errorTok;
    }

    advance(); // consuma '

    Token tok;
    tok.type = TokenType::CharLiteral;
    tok.lexeme = std::string(1, ch);
    tok.position = pos;
    return tok;
}