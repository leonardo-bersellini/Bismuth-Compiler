#ifndef LEXEMES_H
#define LEXEMES_H

#include <unordered_map>
#include <string>

#include "core/tokens.h"

/*
 * Tabella degli operatori lessicali: lexemes che costituiscono un token.
 */

inline const std::unordered_map<std::string, TokenType> operatorsTable = {
    {"+", TokenType::Plus},       {"+=", TokenType::PlusEqual},
    {"-", TokenType::Minus},      {"-=", TokenType::MinusEqual},
    {"*", TokenType::Star},       {"*=", TokenType::StarEqual},
    {"/", TokenType::Slash},      {"/=", TokenType::SlashEqual},
    {"%", TokenType::Percent},
    {"=", TokenType::Equal},      {"==", TokenType::EqualEqual},
    {"!", TokenType::LogicalNot}, {"!=", TokenType::NotEqual},
    {"&&", TokenType::LogicalAnd},
    {"||", TokenType::LogicalOr},
    {">", TokenType::Greater},    {">=", TokenType::GreaterEqual},
    {"<", TokenType::Less},       {"<=", TokenType::LessEqual},
    {"(", TokenType::LParen},     {")", TokenType::RParen},
    {"[", TokenType::LBracket},   {"]", TokenType::RBracket},
    {"{", TokenType::LBrace},     {"}", TokenType::RBrace},
    {";", TokenType::Semicolon},
    {":", TokenType::Colon},      {"::", TokenType::ColonColon},
    {",", TokenType::Comma},
};

inline const std::size_t getMaxOperatorLen() 
{
    std::size_t m = 0;
    for (const auto& entry : operatorsTable)
        m = std::max(m, entry.first.size());
    return m;
};

#endif //LEXEMES_H