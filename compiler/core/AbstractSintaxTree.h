#ifndef ABSTRACTSINTAXTREE_H
#define ABSTRACTSINTAXTREE_H

#include <memory>
#include <vector>
#include <string>
#include <vector>
#include <iostream>

#include "core/tokens.h"
#include "core/symbols.h"

#include "utils/ansi/ansi.h"
#include "utils/qualified_names/qualified_names.h"

// AST Node - classe base da cui ereditano gli oggetti base dell'ast

class ASTNode {
public:
    virtual ~ASTNode() = default;
    TextPosition position;
};

// Expressions - produce un valore

class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
    // lvalue indica un valore al quale si può assegnare un altro valore (rvalue)
    virtual bool isLValue() const =0;
    // true se l'espressione è di natura costante (e.g. literal)
    virtual bool isConstantExpr() const =0;
};

class NumberExpr : public Expr {
public:
    double value;
    bool isInteger; //distinzione double-int

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return true; }
};

class CharExpr : public Expr {
public:
    char value;
    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return true; }
};

class BooleanExpr : public Expr {
public:
    bool value;
    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return true; }
};

class AssignmentExpr : public Expr {
public:
    std::string target_name; //nome letterale del target (cache)
    std::unique_ptr<Expr> target;  //target = value
    std::unique_ptr<Expr> value;

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

// assegnazioni composte con operatori (es: +=)
class OpComposedAssignmentExpr : public Expr {
public:
    TokenType op; 
    std::unique_ptr<AssignmentExpr> assignment;

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

class VariableExpr : public Expr {
public:
    std::string name;
    Qualifiers qualifiers;

    bool isLValue() const override { return true; }
    bool isConstantExpr() const override { return false; }
};

class BinaryExpr : public Expr {
public:
    TokenType op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

class UnaryExpr : public Expr {
public:
    TokenType op;
    std::unique_ptr<Expr> operand;

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

class CallExpr : public Expr { //chiamata ad una funzione
public:
    std::string name;
    Qualifiers qualifiers;
    std::vector<std::unique_ptr<Expr>> args;

    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

//array letterale := [value, value, value, ...]
class LiteralArrayExpr : public Expr {
public:
    mutable ArrayType type;
    std::vector<std::unique_ptr<Expr>> elements;

    bool isLValue() const override { return false; }

    bool isConstantExpr() const override { 
        for(const auto& e : elements) {
            if(!e->isConstantExpr()) return false;
        }
        return true;
    }
};

class ArrayAccessExpr : public Expr {
public:
    std::unique_ptr<Expr> base; //array al quale si sta accedendo
    std::unique_ptr<Expr> index;

    bool isLValue() const override { return true; }
    bool isConstantExpr() const override { return false; }
};

class ErrorExpr : public Expr {
public:
    //void, expression placeholder
    bool isLValue() const override { return false; }
    bool isConstantExpr() const override { return false; }
};

// Statements - esecuzione di azioni

class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
};

class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;
};

class DeclarationStmt : public Stmt {
public:
    Type type;
    bool isConst = false;
    std::string name;
    std::unique_ptr<Expr> initializer; //contiene le informazioni di un'eventuale inizializzazione
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition; //boolean condition
    std::unique_ptr<Stmt> thenBranch; //contenuto tra {}
    std::unique_ptr<Stmt> elseBranch; //può contenere un altro if
};

class ForStmt : public Stmt {
public:
    std::unique_ptr<Stmt> init;
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> update;
    std::unique_ptr<Stmt> body;
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

class BreakStmt : public Stmt {
};

class ContinueStmt : public Stmt {
};

// Switch

class CaseStmt : public Stmt {
public:  
    std::unique_ptr<Expr> label; //condizione
    /*
     * il body può contenere un altro case, ed in quel caso si tratta dell'unico stmt
     * contenuto in esso. oppure una serie indefinita di stmt.
     */
    std::vector<std::unique_ptr<Stmt>> body; //può puntare ad un altro stmt
};

class DefaultStmt: public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> body;
};

class SwitchStmt : public Stmt {
public:
    std::unique_ptr<Expr> scrutinee; //espressione controllata su ogni case
    std::vector<std::unique_ptr<CaseStmt>> cases;
    std::unique_ptr<DefaultStmt> _default;
};

class NamespaceStmt : public Stmt {
public:
    std::string name; 
    std::vector<std::unique_ptr<Stmt>> body;
};

class PrintStmt : public Stmt {
public:
    std::unique_ptr<Expr> content;
};

class ErrorStmt : public Stmt {
public:
    //void, placeholder per error stmt
};

// Functions

struct FunctionParam {
public:
    explicit FunctionParam(const Type& t, const std::string& s, const bool& c)
            : type(t), name(s), isConst(c) {}
            
    Type type;
    std::string name;
    bool isConst = false;
};

class FunctionStmt : public Stmt {
public:
    std::string name;
    Type returnType;
    std::vector<FunctionParam> params;
    std::unique_ptr<Stmt> body; // BlockStmt
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> value; //nullptr per 'return;'
};

// Program

class Program {
public:
    Program() {}
    std::vector<std::unique_ptr<Stmt>> statements;
};

#endif // ABSTRACTSINTAXTREE_H
