#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>
#include "parsing.h"
#include "bytecode.h"
class VM {
public:
    VM(const std::vector<Instruction>& code,
       const std::vector<std::string>& names)
        : code_(code), names_(names) {}

    void run() {
        ip_ = 0;
        stack_.clear();
        vars_.clear();

        while (ip_ < code_.size()) {
            const Instruction& ins = code_[ip_++];

            switch (ins.op) {
            //stack
            case Codebase::PUSH_INT:
            case Codebase::PUSH_BOOL:
                stack_.push_back(ins.operand);
                break;

            case Codebase::POP:
                pop();
                break;

            // Airthmetic
            case Codebase::ADD: { auto b = pop(); auto a = pop(); stack_.push_back(a + b); break; }
            case Codebase::SUB: { auto b = pop(); auto a = pop(); stack_.push_back(a - b); break; }
            case Codebase::MUL: { auto b = pop(); auto a = pop(); stack_.push_back(a * b); break; }
            case Codebase::DIV: {
                auto b = pop(); auto a = pop();
                if (b == 0) throw std::runtime_error("VM: division by zero");
                stack_.push_back(a / b);
                break;
            }

            //Comparison
            case Codebase::CMP_GT: { auto b = pop(); auto a = pop(); stack_.push_back(a >  b ? 1 : 0); break; }
            case Codebase::CMP_LT: { auto b = pop(); auto a = pop(); stack_.push_back(a <  b ? 1 : 0); break; }
            case Codebase::CMP_GE: { auto b = pop(); auto a = pop(); stack_.push_back(a >= b ? 1 : 0); break; }
            case Codebase::CMP_LE: { auto b = pop(); auto a = pop(); stack_.push_back(a <= b ? 1 : 0); break; }
            case Codebase::CMP_EQ: { auto b = pop(); auto a = pop(); stack_.push_back(a == b ? 1 : 0); break; }
            case Codebase::CMP_NE: { auto b = pop(); auto a = pop(); stack_.push_back(a != b ? 1 : 0); break; }

            //Variables
            case Codebase::LOAD_VAR: {
                int idx = ins.operand;
                auto it = vars_.find(idx);
                if (it == vars_.end())
                    stack_.push_back(0);
                else
                    stack_.push_back(it->second);
                break;
            }
            case Codebase::STORE_VAR: {
                int val = stack_.back();
                vars_[ins.operand] = val;
                break;
            }

            //control
            case Codebase::JUMP:
                ip_ = static_cast<size_t>(ins.operand);
                break;

            case Codebase::JUMP_IF_FALSE: {
                int cond = pop();
                if (cond == 0)
                    ip_ = static_cast<size_t>(ins.operand);
                break;
            }

            // print,input
            case Codebase::PRINT:
                std::cout << pop() << std::endl;
                break;

            case Codebase::INPUT: {
                int val;
                std::cin >> val;
                stack_.push_back(val);
                break;
            }
            case Codebase::HALT:
                return;
            }
        }
    }

private: 
    const std::vector<Instruction>& code_;
    const std::vector<std::string>& names_;
    std::vector<int>stack_;
    std::unordered_map<int, int>vars_;
    size_t ip_ = 0;

    int pop() {
        if (stack_.empty())
            throw std::runtime_error("VM: stack underflow");
        int val = stack_.back();
        stack_.pop_back();
        return val;
    }
};
