#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>
#include "parsing.h"
enum class Codebase {PUSH_INT,PUSH_BOOL,POP,ADD,SUB,MUL,DIV,CMP_GT,CMP_LT,CMP_GE,CMP_LE,CMP_EQ,CMP_NE,
    LOAD_VAR,STORE_VAR,JUMP,JUMP_IF_FALSE,PRINT,INPUT,HALT,
};

struct Instruction {
    Codebase op;
    int operand;
    Instruction(Codebase o, int arg = 0) : op(o), operand(arg) {}
};

class Compiler {
public:
    void compile(const Expr& node) {
        emit(node);
        code_.push_back({Codebase::HALT});
    }
    const std::vector<Instruction>& getCode()  const { return code_;  }
    const std::vector<std::string>& getNames() const { return names_; }
    //debugg
    void disassemble() const {
        static const char* opNames[] = {
            "PUSH_INT", "PUSH_BOOL", "POP",
            "ADD", "SUB", "MUL", "DIV",
            "CMP_GT", "CMP_LT", "CMP_GE", "CMP_LE", "CMP_EQ", "CMP_NE",
            "LOAD_VAR", "STORE_VAR",
            "JUMP", "JUMP_IF_FALSE",
            "PRINT", "INPUT",
            "HALT",
        };
        //printing
        std::cout << "\nBytecode\n";
        for (size_t i = 0; i < code_.size(); ++i) {
            const auto& ins = code_[i];
            std::cout << std::setw(4) << i << "  "
                      << std::setw(16) << std::left
                      << opNames[static_cast<int>(ins.op)];
            switch (ins.op) {
                case Codebase::PUSH_INT:
                case Codebase::PUSH_BOOL:
                    std::cout << ins.operand;
                    break;
                case Codebase::LOAD_VAR:
                case Codebase::STORE_VAR:
                    std::cout << ins.operand
                              << "  (" << names_[ins.operand] << ")";
                    break;
                case Codebase::JUMP:
                case Codebase::JUMP_IF_FALSE:
                    std::cout << "->" << ins.operand;
                    break;
                default:
                    break;
            }
            std::cout <<std::endl;
        }
        std::cout << "Done"<<std::endl;
    }

private:
    std::vector<Instruction> code_;
    std::vector<std::string> names_;
    std::unordered_map<std::string, int> nameIndex_;

    // for variable name
    int resolveName(const std::string& name) {
        auto it = nameIndex_.find(name);
        if (it != nameIndex_.end()) return it->second;
        int idx = static_cast<int>(names_.size());
        names_.push_back(name);
        nameIndex_[name] = idx;
        return idx;
    }

    // Emiiting
    size_t emitInstr(Codebase op, int operand = 0) {
        code_.push_back({op, operand});
        return code_.size() - 1;
    }
    void patchJump(size_t instrAddr, int target) {
        code_[instrAddr].operand = target;
    }
    void emit(const Expr& node) {
        if (auto* n = dynamic_cast<const NumberExpr*>(&node)) {
            emitInstr(Codebase::PUSH_INT, n->value);
            return;
        }
        if (auto* n = dynamic_cast<const BooleanExpr*>(&node)) {
            emitInstr(Codebase::PUSH_BOOL, n->value ? 1 : 0);
            return;
        }
        if (auto* n = dynamic_cast<const VariableExpr*>(&node)) {
            int idx = resolveName(n->name);
            emitInstr(Codebase::LOAD_VAR, idx);
            return;
        }
        if (dynamic_cast<const InputExpr*>(&node)) {
            emitInstr(Codebase::INPUT);
            return;
        }
        if (auto* n = dynamic_cast<const BinaryExpr*>(&node)) {
            emit(*n->left);
            emit(*n->right);
            switch (n->op) {
                case '+': emitInstr(Codebase::ADD); break;
                case '-': emitInstr(Codebase::SUB); break;
                case '*': emitInstr(Codebase::MUL); break;
                case '/': emitInstr(Codebase::DIV); break;
                default:
                    throw std::runtime_error(
                        std::string("Unknown binary operator: ") + n->op);
            }
            return;
        }
        if (auto* n = dynamic_cast<const CompareExpr*>(&node)) {
            emit(*n->left);
            emit(*n->right);
            if(n->op == ">")  emitInstr(Codebase::CMP_GT);
            else if (n->op == "<")  emitInstr(Codebase::CMP_LT);
            else if (n->op == ">=") emitInstr(Codebase::CMP_GE);
            else if (n->op == "<=") emitInstr(Codebase::CMP_LE);
            else if (n->op == "==") emitInstr(Codebase::CMP_EQ);
            else if (n->op == "!=") emitInstr(Codebase::CMP_NE);
            else throw std::runtime_error("Unknown comparison operator: " + n->op);
            return;
        }
        if (auto* n = dynamic_cast<const AssignExpr*>(&node)) {
            emit(*n->value);
            int idx = resolveName(n->name);
            emitInstr(Codebase::STORE_VAR, idx);
            return;
        }
        if (auto* n = dynamic_cast<const PrintExpr*>(&node)) {
            emit(*n->value);
            emitInstr(Codebase::PRINT);
            return;
        }
        if (auto* n = dynamic_cast<const BlockExpr*>(&node)) {
            for (const auto& stmt : n->statements) {
                emit(*stmt);
                if (!dynamic_cast<const IfExpr*>(stmt.get()) &&
                    !dynamic_cast<const WhileExpr*>(stmt.get()) &&
                    !dynamic_cast<const PrintExpr*>(stmt.get()) &&
                    !dynamic_cast<const BlockExpr*>(stmt.get())) {
                    emitInstr(Codebase::POP);
                }
            }
            return;
        }
        if (auto* n = dynamic_cast<const IfExpr*>(&node)) {
            emit(*n->condition);
            size_t jumpToElse = emitInstr(Codebase::JUMP_IF_FALSE, 0);

            emit(*n->thenBranch);

            if (n->elseBranch) {
                size_t jumpToEnd = emitInstr(Codebase::JUMP, 0);
                patchJump(jumpToElse, static_cast<int>(code_.size()));
                emit(*n->elseBranch);
                patchJump(jumpToEnd, static_cast<int>(code_.size()));
            } else {
                patchJump(jumpToElse, static_cast<int>(code_.size()));
            }
            return;
        }
        if (auto* n = dynamic_cast<const WhileExpr*>(&node)) {
            int loopStart = static_cast<int>(code_.size());
            emit(*n->condition);
            size_t jumpToEnd = emitInstr(Codebase::JUMP_IF_FALSE, 0);
            emit(*n->body);
            emitInstr(Codebase::JUMP, loopStart);
            patchJump(jumpToEnd, static_cast<int>(code_.size()));
            return;
        }

        throw std::runtime_error("Compiler: unknown AST node type");
    }
};