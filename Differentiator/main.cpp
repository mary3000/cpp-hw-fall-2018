#include <iostream>
#include "src/DiffFunc.h"
#include "src/Differentiator.h"

void TexDiff(const std::string& func, const std::string& name) {
    Differentiator d = Differentiator(func);
    d.Differentiate();
    d.DiffToString(name);
}

int main() {
    TexDiff("cos(x)", "cos1");
    TexDiff("cos(-x*5)", "cos2");
    TexDiff("cos(-x)", "cos3");

    TexDiff("x/(x+5)", "div1");

    TexDiff("5", "const1");
    TexDiff("cos(sin(2/3))", "const2");

    TexDiff("x", "x1");

    TexDiff("(x-6)*(2*x+7)", "mult1");

    TexDiff("cos(sin(cos(x+7))) + 42.5", "compl1");
    TexDiff("exp(sin(cos(x+7)))/(x*(3-5))", "compl2");

    TexDiff("exp(-x)", "exp1");

    TexDiff("sqr(sqr(x+1/x))", "sqr1");
    return 0;
}