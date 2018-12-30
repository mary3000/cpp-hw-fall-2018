#include <utility>

#include <utility>

//
// Created by mariafeofanova on 14.12.18.
//

#ifndef DIFFERENTIATOR_DIFFFUNC_H
#define DIFFERENTIATOR_DIFFFUNC_H

#include <cstddef>
#include <unordered_map>
#include <math.h>
#include <assert.h>

const size_t kMaxPriority = 100;
struct DiffFunc {
    DiffFunc() noexcept;
    DiffFunc(size_t pr, size_t args, std::string s) noexcept;

    template <size_t N>
    static std::unordered_map<std::string, DiffFunc*> MakeMap(const std::array<DiffFunc*, N>& arr) noexcept;

    double Operate(double a, double b);
    double Operate(double a);

    static std::unordered_map<std::string, DiffFunc*> kStrToFunc;

    size_t priority;
    size_t num_args;
    std::string str;
};

DiffFunc X = DiffFunc(kMaxPriority, 0, "x");
DiffFunc PLUS = DiffFunc(1, 2, "+");
DiffFunc MINUS = DiffFunc(1, 2, "-");
DiffFunc UNARY_MINUS = DiffFunc(1, 1, "-");
DiffFunc MULT = DiffFunc(2, 2, "*");
DiffFunc FRAC = DiffFunc(2, 2, "/");
DiffFunc COS = DiffFunc(kMaxPriority, 1, "cos");
DiffFunc SIN = DiffFunc(kMaxPriority, 1, "sin");
DiffFunc EXP = DiffFunc(kMaxPriority, 1, "exp");
DiffFunc SQR = DiffFunc(kMaxPriority, 1, "sqr");
DiffFunc OPEN_BRACE = DiffFunc(0, 0, "(");
DiffFunc CLOSE_BRACE = DiffFunc(kMaxPriority, 0, ")");
DiffFunc CONST = DiffFunc(kMaxPriority, 0, "CONST");

const std::array kFuncs = {
        &X, &PLUS, &MINUS, &MULT, &FRAC, &COS, &SIN, &EXP, &SQR, &OPEN_BRACE, &CLOSE_BRACE
};

std::unordered_map<std::string, DiffFunc*> DiffFunc::kStrToFunc = MakeMap(kFuncs);

std::ostream &operator<<(std::ostream &os, const DiffFunc& func) {
    if (&func == &SQR) {
        return os << "^2 ";
    }
    if (func.num_args == 1 && &func != &UNARY_MINUS) {
        os << "\\";
    }
    if (&func == &MULT) {
        return os << " \\cdot ";
    }
    return os << func.str;
}

DiffFunc::DiffFunc() noexcept : DiffFunc(0, 0, "") {}

template<size_t N>
std::unordered_map<std::string, DiffFunc*> DiffFunc::MakeMap(const std::array<DiffFunc*, N> &arr) noexcept {
    std::unordered_map<std::string, DiffFunc*> map;
    for (const auto &i : arr) {
        map[i->str] = i;
    }
    return map;
}

DiffFunc::DiffFunc(size_t pr, size_t args, std::string s) noexcept : priority(pr),
                                                            num_args(args),
                                                            str(std::move(s)) {}

double DiffFunc::Operate(double a, double b) {
    assert(num_args == 2);
    if (this == &PLUS) {
        return a + b;
    }
    if (this == &MULT) {
        return a * b;
    }
    if (this == &MINUS) {
        return a - b;
    }
    if (this == &FRAC) {
        return a / b;
    }
    assert(false);
}

double DiffFunc::Operate(double a) {
    assert(num_args == 1);
    if (this == &COS) {
        return std::cos(a);
    }
    if (this == &SIN) {
        return std::sin(a);
    }
    if (this == &EXP) {
        return std::exp(a);
    }
    if (this == &UNARY_MINUS) {
        return -a;
    }
    assert(false);
}


#endif //DIFFERENTIATOR_DIFFFUNC_H
