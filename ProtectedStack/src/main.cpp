#include <iostream>
#include "stack.h"

int main() {
    Stack<int> stack;
    const int c = 100;
    for (int i = 1; i < c; i++) {
        stack.Push(i);
    }
    *((int*)(&stack)) = 0;
    int a = 0;
    for (int i = 1; i < c; i++) {
        bool popped = stack.Pop(a);
        if (!popped) {
            std::cout << "Popped: failed" << std::endl;
        } else {
            std::cout << a << std::endl;
        }
    }
    int b = 25;
    for (int i = 1; i < b; i++) {
        stack.Push(i);
    }
    for (int i = 1; i < b; i++) {
        bool popped = stack.Pop(a);
        if (!popped) {
            std::cout << "Popped: failed" << std::endl;
        } else {
            std::cout << a << std::endl;
        }
    }
    assert(!stack.Pop(a));
    return 0;
}
