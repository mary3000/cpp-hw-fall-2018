#ifndef PROCESSOR_PROCESSOR_H
#define PROCESSOR_PROCESSOR_H

#include <map>
#include "../../ProtectedStack/src/stack.h"

#define REGISTERS_SIZE 4

enum Command {
    PUSH, PUSHR, POP, POPR, DUP, SWP, MOV, MOVD, IN, OUT, MUL, ADD, MOD, JMP, JE, JNE, END, HLT
};

enum Register {
    RAX, RBX, RCX, RDX
};

std::map<std::string, Command> kStringToCommands {
        {"push", PUSH},
        {"pop", POP},
        {"dup", DUP},
        {"swp", SWP},
        {"mov", MOV},
        {"in", IN},
        {"out", OUT},
        {"mul", MUL},
        {"add", ADD},
        {"mod", MOD},
        {"jmp", JMP},
        {"je", JE},
        {"jne", JNE},
        {"end", END},
        {"hlt", HLT}
};

std::map<std::string, Register > kStringToRegisters {
        {"RAX", RAX},
        {"RBX", RBX},
        {"RCX", RCX},
        {"RDX", RDX}
};

class Processor {
public:
    Processor(std::istream &input, int size);
    ~Processor();
    void Run(std::istream *in, std::ostream &out);
private:
    void Parse(std::istream &input);
    void Duplicate(int num);
    void GetMarks(std::istream& input);

    Stack<int> stack;
    int* program;
    std::map<std::string, int> marks;
    int registers[REGISTERS_SIZE];
    int max_program_size;
    int program_size;
};


Processor::Processor(std::istream &input, int size) {
    max_program_size = size;
    program = new int[max_program_size];
    Parse(input);
}

void Processor::Parse(std::istream &input) {
    GetMarks(input);
    input.clear();
    input.seekg(0, input.beg);
    std::string temp;
    Command cmd;
    int j = 0;
    while (getline(input, temp, '\n')) {
        size_t offset = temp.find(' ');
        std::string cmd_str = temp.substr(0, offset);
        std::string value_str = offset != std::string::npos ? temp.substr(offset + 1) : "";
        if (kStringToCommands.find(cmd_str) != kStringToCommands.end()) {
            cmd = kStringToCommands[cmd_str];
            if (cmd == PUSH || cmd == JMP || cmd == JE || cmd == JNE || cmd == DUP || cmd == MOV) {
                assert(!value_str.empty() && "expected: operand");
            } else if (cmd != POP) {
                assert(value_str.empty() && "unexpected operand");
            }
            program[j++] = cmd;
            if (cmd == PUSH || cmd == DUP) {
                if (cmd == PUSH && kStringToRegisters.find(value_str) != kStringToRegisters.end()) {
                    Register r = kStringToRegisters[value_str];
                    program[j - 1] = PUSHR;
                    program[j++] = r;
                } else {
                    program[j++] = std::stoi(value_str);
                }
            } else if (cmd == JMP || cmd == JE || cmd == JNE) {
                program[j++] = marks[value_str];
            } else if (cmd == MOV) {
                size_t offset1 = temp.find(' ', offset + 1);
                assert(offset != std::string::npos && "expected: operand");
                std::string value_str1 = temp.substr(offset + 1, offset1 - offset - 1);
                std::string value_str2 = temp.substr(offset1 + 1);
                assert(kStringToRegisters.find(value_str1) != kStringToRegisters.end());
                Register r1 = kStringToRegisters[value_str1];
                program[j++] = r1;
                if (kStringToRegisters.find(value_str2) != kStringToRegisters.end()) {
                    Register r2 = kStringToRegisters[value_str2];
                    program[j++] = r2;
                } else {
                    program[j - 2] = MOVD;
                    program[j++] = std::stoi(value_str2);
                }
            } else if (cmd == POP && !value_str.empty()) {
                program[j - 1] = POPR;
                assert(kStringToRegisters.find(value_str) != kStringToRegisters.end());
                Register r = kStringToRegisters[value_str];
                program[j++] = r;
            }
        } else {
            assert(cmd_str[cmd_str.length() - 1] == ':' && "unknown command");
            program[j++] = HLT;
        }
    }
    program_size = j;
}

void Processor::Run(std::istream *in, std::ostream &out) {
    int tmp1;
    int tmp2;
    int i = 0;
    while (i < program_size) {
        switch (program[i]) {
            case PUSH:
                stack.Push(program[++i]);
                break;
            case PUSHR:
                stack.Push(registers[program[++i]]);
                break;
            case POP:
                assert(stack.Pop());
                break;
            case POPR:
                assert(stack.Pop(tmp1));
                registers[program[++i]] = tmp1;
                break;
            case DUP:
                tmp1 = program[++i];
                Duplicate(tmp1);
                break;
            case SWP:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                stack.Push(tmp1);
                stack.Push(tmp2);
                break;
            case MOV:
                registers[program[i + 1]] = registers[program[i + 2]];
                i += 2;
                break;
            case MOVD:
                registers[program[i + 1]] = program[i + 2];
                i += 2;
                break;
            case IN:
                assert(in && "input stream should be specified!");
                *in >> tmp1;
                stack.Push(tmp1);
                break;
            case OUT:
                assert(stack.Pop(tmp1));
                out << tmp1 << std::endl;
                break;
            case MUL:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                stack.Push(tmp1 * tmp2);
                break;
            case ADD:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                stack.Push(tmp1 + tmp2);
                break;
            case MOD:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                stack.Push(tmp2 % tmp1);
                break;
            case JMP:
                i = program[i + 1];
                continue;
            case JE:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                if (tmp1 == tmp2) {
                    i = program[i + 1];
                    continue;
                } else {
                    i++;
                }
                break;
            case JNE:
                assert(stack.Pop(tmp1));
                assert(stack.Pop(tmp2));
                if (tmp1 != tmp2) {
                    i = program[i + 1];
                    continue;
                } else {
                    i++;
                }
                break;
            case END:
                return;
            default:
                break;
        }
        i++;
    }
}

Processor::~Processor() {
    delete[] program;
}

void Processor::Duplicate(int num) {
    int tmp1, tmp2;
    assert(stack.Pop(tmp1));
    assert(stack.Pop(tmp2));
    while (--num >= 0) {
        stack.Push(tmp2);
        stack.Push(tmp1);
    }
}

void Processor::GetMarks(std::istream &input) {
    std::string temp;
    int j = 0;
    while (getline(input, temp, '\n')) {
        j++;
        size_t offset = temp.find(':');
        if (offset == std::string::npos) {
            offset = temp.find(' ');
            while (offset != std::string::npos) {
                j++;
                offset = temp.find(' ', offset + 1);
            }
            continue;
        }
        std::string value_str = temp.substr(offset);
        assert(value_str == ":" && "unexpected operand for mark");
        std::string cmd_str = temp.substr(0, offset);
        marks[cmd_str] = j;
    }
}

#endif //PROCESSOR_PROCESSOR_H
