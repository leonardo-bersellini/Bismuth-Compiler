#ifndef PRETTY_PRINT_AST_H
#define PRETTY_PRINT_AST_H

#include <iostream>
#include <string>

#include "AbstractSintaxTree.h"

inline void printAST(const Expr* node, int depth = 0) {
    std::string indent(depth * 2, ' ');

    if (auto n = dynamic_cast<const NumberExpr*>(node)) {
        std::cout << indent << "NumberExpr:" << n->value << std::endl;
    }
    else if (auto n = dynamic_cast<const CharExpr*>(node)) {
        std::cout << indent << "CharExpr:" << n->value << std::endl;
    }
    else if (auto n = dynamic_cast<const BooleanExpr*>(node)) {
        std::cout << indent << "BooleanExpr:" << (n->value ? "true" : "false") << std::endl;
    }
    else if (auto n = dynamic_cast<const AssignmentExpr*>(node)) {
        std::cout << indent << "AssignmentExpr:" << n->target_name << std::endl;
        printAST(n->target.get(), depth +1);
        printAST(n->value.get(), depth +1);
    }
    else if(auto n = dynamic_cast<const OpComposedAssignmentExpr*>(node)) {
        std::cout << indent << "ComposedAssignmentExpr:" << typeToString(n->op) << std::endl;
        printAST(n->assignment.get(), depth +1);
    }
    else if (auto n = dynamic_cast<const VariableExpr*>(node)) {
        std::cout << indent << "VariableExpr:" << n->name << std::endl;
    }
    else if (auto n = dynamic_cast<const UnaryExpr*>(node)) {
        std::cout << indent << "UnaryExpr:" << typeToString(n->op) << std::endl;
        printAST(n->operand.get(), depth + 1);
    }
    else if (auto n = dynamic_cast<const BinaryExpr*>(node)) {
        std::cout << indent << "BinaryExpr:" << typeToString(n->op) << std::endl;
        printAST(n->left.get(), depth + 1);
        printAST(n->right.get(), depth + 1);
    }
    else if (auto n = dynamic_cast<const CallExpr*>(node)) {
        std::cout << indent << "CallExpr:" << n->name << std::endl;

        for (const auto& arg : n->args)
            printAST(arg.get(), depth + 1);
    }
    else if (auto n = dynamic_cast<const LiteralArrayExpr*>(node)) {
        std::cout << indent << "LiteralArrayExpr" << std::endl;

        for (const auto& el : n->elements)
            printAST(el.get(), depth + 1);
    }
    else if (auto n = dynamic_cast<const ArrayAccessExpr*>(node)) {
        std::cout << indent << "ArrayAccessExpr" << std::endl;

        std::cout << std::string((depth + 1) * 2, ' ') << "Base:" << std::endl;
        printAST(n->base.get(), depth + 2);

        std::cout << std::string((depth + 1) * 2, ' ') << "Index:" << std::endl;
        printAST(n->index.get(), depth + 2);
    }
    else if (dynamic_cast<const ErrorExpr*>(node)) {
        std::cout << indent << "ErrorExpr" << std::endl;
    }
    else {
        std::cout << indent << "Unknown Expr" << std::endl;
    }
}

inline void printStmt(const Stmt* stmt, int depth = 0) 
{
    std::cout << ansi::color::bright_black;

    if (!stmt) {
        std::cout << std::string(depth * 2, ' ') + "<null stmt>" << std::endl;
        return;
    }

    std::string indent(depth * 2, ' ');

    if (auto s = dynamic_cast<const ExpressionStmt*>(stmt)) {

        std::cout << indent << "ExpressionStmt" << std::endl;
        printAST(s->expr.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const DeclarationStmt*>(stmt)) {

        std::cout << indent << "DeclarationStmt:" << s->name << std::endl;

        if (s->initializer)
            printAST(s->initializer.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const BlockStmt*>(stmt)) {

        std::cout << indent << "BlockStmt" << std::endl;

        for (const auto& st : s->statements)
            printStmt(st.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const IfStmt*>(stmt)) {

        std::cout << indent << "IfStmt";

        std::cout << std::string((depth + 1) * 2, ' ') << "Condition:" << std::endl;
        printAST(s->condition.get(), depth + 2);

        std::cout << std::string((depth + 1) * 2, ' ') << "Then:" << std::endl;
        printStmt(s->thenBranch.get(), depth + 2);

        if (s->elseBranch) {
            std::cout << std::string((depth + 1) * 2, ' ') << "Else:" << std::endl;
            printStmt(s->elseBranch.get(), depth + 2);
        }
    }

    else if (auto s = dynamic_cast<const WhileStmt*>(stmt)) {

        std::cout << indent << "WhileStmt" << std::endl;

        std::cout << std::string((depth + 1) * 2, ' ') << "Condition:" << std::endl;
        printAST(s->condition.get(), depth + 2);

        std::cout << std::string((depth + 1) * 2, ' ') << "Body:" << std::endl;
        printStmt(s->body.get(), depth + 2);
    }

    else if (auto s = dynamic_cast<const ForStmt*>(stmt)) {

        std::cout << indent << "ForStmt" << std::endl;

        if (s->init) {
            std::cout << std::string((depth + 1) * 2, ' ') << "Init:" << std::endl;
            printStmt(s->init.get(), depth + 2);
        }

        if (s->condition) {
            std::cout << std::string((depth + 1) * 2, ' ') << "Condition:" << std::endl;
            printAST(s->condition.get(), depth + 2);
        }

        if (s->update) {
            std::cout << std::string((depth + 1) * 2, ' ') << "Update:" << std::endl;
            printAST(s->update.get(), depth + 2);
        }

        if (s->body) {
            std::cout << std::string((depth + 1) * 2, ' ') << "Body:" << std::endl;
            printStmt(s->body.get(), depth + 2);
        }
    }

    else if(auto s = dynamic_cast<const PrintStmt*>(stmt)) {
        std::cout << indent << "PrintStmt:" << std::endl;
        std::cout << std::string((depth + 1) * 2, ' ') << "Content:" << std::endl;
        printAST(s->content.get(), depth +2);
    }

    else if (auto s = dynamic_cast<const SwitchStmt*>(stmt)) {

        std::cout << indent << "SwitchStmt" << std::endl;

        std::cout << std::string((depth + 1) * 2, ' ') << "Scrutinee:" << std::endl;
        printAST(s->scrutinee.get(), depth + 2);

        for (const auto& c : s->cases)
            printStmt(c.get(), depth + 1);

        if (s->_default)
            printStmt(s->_default.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const CaseStmt*>(stmt)) {

        std::cout << indent << "CaseStmt" << std::endl;

        std::cout << std::string((depth + 1) * 2, ' ') << "Label:" << std::endl;
        printAST(s->label.get(), depth + 2);

        for (const auto& st : s->body)
            printStmt(st.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const DefaultStmt*>(stmt)) {

        std::cout << indent << "DefaultStmt" << std::endl;

        for (const auto& st : s->body)
            printStmt(st.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const NamespaceStmt*>(stmt)) {

        std::cout << indent << "NamespaceStmt:" << s->name << std::endl;

        for (const auto& st : s->body)
            printStmt(st.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const FunctionStmt*>(stmt)) {

        std::cout << indent << "FunctionStmt:" << s->name << std::endl;

        for (const auto& p : s->params)
            std::cout << std::string((depth + 1) * 2, ' ') << "Param:" << p.name << std::endl;

        printStmt(s->body.get(), depth + 1);
    }

    else if (auto s = dynamic_cast<const ReturnStmt*>(stmt)) {

        std::cout << indent << "ReturnStmt" << std::endl;

        if (s->value)
            printAST(s->value.get(), depth + 1);
    }

    else if (dynamic_cast<const BreakStmt*>(stmt)) {

        std::cout << indent << "BreakStmt" << std::endl;
    }

    else if (dynamic_cast<const ContinueStmt*>(stmt)) {

        std::cout << indent << "ContinueStmt" << std::endl;
    }

    else if (dynamic_cast<const ErrorStmt*>(stmt)) {

        std::cout << indent << "ErrorStmt" << std::endl;
    }

    else {

        std::cout << indent << "Unknown Stmt" << std::endl;
    }

    std::cout << ansi::color::reset;
}




#endif //PRETTY_PRINT_AST_H