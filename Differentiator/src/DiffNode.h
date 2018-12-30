#ifndef DIFFERENTIATOR_NODE_H
#define DIFFERENTIATOR_NODE_H

#include <string>
#include <memory>
#include "DiffFunc.h"

struct DiffNode {
    DiffNode() : func(&CONST), value(0), left(nullptr), right(nullptr) {}
    DiffNode(DiffFunc* func_) : DiffNode(func_, 0, false) {}
    DiffNode(DiffFunc* func_, double value_) : DiffNode(func_, value_, false) {}
    DiffNode(DiffFunc* func_, double value_, bool negative_) : func(func_), value(value_) {}
    DiffNode(DiffFunc* func_, std::shared_ptr<DiffNode> l, std::shared_ptr<DiffNode> r) : DiffNode(func_, l, r, 0) {}
    DiffNode(DiffFunc* func_, std::shared_ptr<DiffNode> l, std::shared_ptr<DiffNode> r,
            double value_) : func(func_), value(value_), left(l), right(r) {}
    DiffNode(const DiffNode& other);
    DiffNode& operator=(const DiffNode& other);
    void Refresh();
    void ToString(std::stringstream& s);

    DiffFunc* func;
    double value;

    std::shared_ptr<DiffNode> left;
    std::shared_ptr<DiffNode> right;

    bool IsValueOf(double val) const;
};

void DiffNode::Refresh() {
    if (left != nullptr) {
        left->Refresh();
    }
    if (right != nullptr) {
        right->Refresh();
    }
    if (func == nullptr) {
        return;
    }
    bool changed = false;
    if (func->num_args == 2 && left->func == &CONST && right->func == &CONST) {
        value = func->Operate(left->value, right->value);
        if (value < 0) {
            func = &UNARY_MINUS;
            left->value = -value;
            value = 0;
        } else {
            func = &CONST;
            left = nullptr;
        }
        right = nullptr;
    }
    if (func->num_args == 1 && left->func == &CONST && func != &UNARY_MINUS) {
        value = func->Operate(left->value);
        if (value < 0) {
            func = &UNARY_MINUS;
            left->value = -value;
        }
        else {
            func = &CONST;
            left = nullptr;
        }
    }
    if (func == &MULT && left->IsValueOf(1)) {
        *this = DiffNode(*right);
    }
    if (func == &MULT && right->IsValueOf(1)) {
        *this = DiffNode(*left);
    }
    if (func == &MULT && (left->IsValueOf(0) || right->IsValueOf(0))) {
        func = &CONST;
        value = 0;
    }
    if ((func == &PLUS || func == &MINUS) && (left->IsValueOf(0) || right->IsValueOf(0))
    && !(func == &MINUS && left->IsValueOf(0))) {
        *this = DiffNode((left->IsValueOf(0) ? *right : *left));
    }
    if ((func == &PLUS || func == &MINUS) && right->func == &UNARY_MINUS) {
        func = func == &PLUS ? &MINUS : &PLUS;
        *right = *right->left;
        changed = true;
    }
    if (func == &UNARY_MINUS && left->func == &UNARY_MINUS) {
        DiffNode tmp(*left->left);
        *this = tmp;
        changed = true;
    }
    if (func->num_args == 2 && func->priority > UNARY_MINUS.priority && (left->func == &UNARY_MINUS || right->func == &UNARY_MINUS)) {
        bool negate = false;
        if (left->func == &UNARY_MINUS) {
            left = std::make_shared<DiffNode>(*left->left);
            negate = !negate;
        }
        if (right->func == &UNARY_MINUS) {
            right = std::make_shared<DiffNode>(*right->left);
            negate = !negate;
        }
        if (negate) {
            left = std::make_shared<DiffNode>(*this);
            func = &UNARY_MINUS;
            right = nullptr;
        }
        changed = true;
    }
    if (changed) {
        Refresh();
    }
}

bool DiffNode::IsValueOf(double val) const {
    return func == &CONST && value == val;
}

void DiffNode::ToString(std::stringstream &s) {
    if (func->num_args == 2 && func != &FRAC) {
        if (left->func->priority < func->priority) {
            s << OPEN_BRACE;
        }
        left->ToString(s);
        if (left->func->priority < func->priority) {
            s << CLOSE_BRACE;
        }
        s << *func;
        if (right->func->priority < func->priority) {
            s << OPEN_BRACE;
        }
        right->ToString(s);
        if (right->func->priority < func->priority) {
            s << CLOSE_BRACE;
        }
    } else if (func == &FRAC) {
        s << "\\frac{";
        left->ToString(s);
        s << "}{";
        right->ToString(s);
        s << "}";
    } else if (func == &X) {
        s << *func;
    } else if (func == &CONST) {
        s << value;
    } else if (func == &SQR) {
        s << OPEN_BRACE;
        left->ToString(s);
        s << CLOSE_BRACE;
        s << *func;
    } else if (func->num_args == 1) {
        s << *func;
        if (func != &UNARY_MINUS || func->priority >= left->func->priority) {
            s << OPEN_BRACE;
        }
        left->ToString(s);
        if (func != &UNARY_MINUS || func->priority >= left->func->priority) {
            s << CLOSE_BRACE;
        }
    }
}

DiffNode::DiffNode(const DiffNode &other) {
    *this = other;
}

DiffNode &DiffNode::operator=(const DiffNode &other) {
    func = other.func;
    value = other.value;
    if (other.left != nullptr) {
        left = std::make_shared<DiffNode>(*other.left);
    }
    if (other.right != nullptr) {
        right = std::make_shared<DiffNode>(*other.right);
    }
    return *this;
}


#endif //DIFFERENTIATOR_NODE_H
