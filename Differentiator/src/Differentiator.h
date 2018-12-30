#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "DiffNode.h"


const static std::string kTexBegin = "\\documentclass[12pt]{article}\n"
                                     " \n"
                                     "\\begin{document}\n"
                                     "\\title{Differentiator}\n"
                                     "\\author{Mary Feofanova\\\\\n"
                                     "System Programming}\n"
                                     " \n"
                                     "\\maketitle\n"
                                     "\\begin{center}\n"
                                     "Input(function):\n"
                                     "\\end{center}\n"
                                     "$$";
const static std::string kTexMiddle = "$$\n"
                                      "\\begin{center}\n"
                                      "Output(derivative):\n"
                                      "\\end{center}\n"
                                      "$$";
const static std::string kTexEnd = "$$\n"
                                   "\\end{document}";

class Differentiator {
public:
    explicit Differentiator(const std::string& in);

    void Differentiate();
    void DiffToString(const std::string &file_name);
private:
    void BuildTree(DiffNode *node, size_t l, size_t r);
    void Split();
    std::shared_ptr<DiffNode> GetDiff(DiffNode *from);
    std::shared_ptr<DiffNode> ComplexDiff(DiffNode *from, DiffFunc *op);

    std::unique_ptr<DiffNode> root;
    std::shared_ptr<DiffNode> diff_root;
    std::string input;
    std::vector<DiffFunc*> funcs;
    std::vector<double> values;
};

Differentiator::Differentiator(const std::string &in) {
    input = in;
    root = std::make_unique<DiffNode>();
    Split();
    BuildTree(root.get(), 0, funcs.size() - 1);
    root->Refresh();
}

void Differentiator::BuildTree(DiffNode *node, size_t l, size_t r) {
    size_t candidate = l;
    size_t balance = 0;
    while (funcs[candidate] == &OPEN_BRACE) {
        candidate++;
    }
    size_t min_balance = 0;
    if (funcs[l] == &OPEN_BRACE) {
        min_balance++;
        for (size_t i = l; i < r; ++i) {
            if (funcs[i] == &OPEN_BRACE) {
                balance++;
            } else if (funcs[i] == &CLOSE_BRACE) {
                min_balance = std::min(min_balance, --balance);
            }
            if (min_balance == 0) {
                break;
            }
        }
    }
    l += min_balance;
    r -= min_balance;
    balance = 0;
    for (size_t i = l; i <= r; ++i) {
        if (funcs[i] == &OPEN_BRACE) {
            balance++;
        } else if (funcs[i] == &CLOSE_BRACE) {
            balance--;
        } else if (balance == 0 && funcs[i]->num_args == 2 && funcs[i]->priority <= funcs[candidate]->priority) {
            candidate = i;
        }
    }
    node->func = funcs[candidate];
    node->value = values[candidate];
    if (node->func->num_args == 2 && candidate > l) {
        node->left = std::make_shared<DiffNode>();
        BuildTree(node->left.get(), l, candidate - 1);
    }
    if (node->func->num_args == 2 && candidate < r) {
        node->right = std::make_shared<DiffNode>();
        BuildTree(node->right.get(), candidate + 1, r);
    }
    if (node->func->num_args == 1) {
        node->left = std::make_shared<DiffNode>();
        BuildTree(node->left.get(), l + 1, r);
    }
}

void Differentiator::Split() {
    std::string tmp;
    bool curr_num = false;
    for (char c : input) {
        if (c >= '0' && c <= '9' || c == '.') {
            curr_num = true;
            tmp.push_back(c);
            continue;
        } else if (curr_num) {
            funcs.push_back(&CONST);
            values.push_back(std::stod(tmp));
            tmp.clear();
            curr_num = false;
        }
        if (c != ' ') {
            tmp.push_back(c);
        }
        if (DiffFunc::kStrToFunc.find(tmp) != DiffFunc::kStrToFunc.end()) {
            if (DiffFunc::kStrToFunc.at(tmp) == &MINUS && (funcs.empty() || funcs.back() == &OPEN_BRACE)) {
                funcs.push_back(&UNARY_MINUS);
            } else {
                funcs.push_back(DiffFunc::kStrToFunc.at(tmp));
            }
            values.push_back(0);
            tmp.clear();
        }
    }
    if (curr_num) {
        funcs.push_back(&CONST);
        values.push_back(std::stod(tmp));
    }
}

void Differentiator::Differentiate() {
    diff_root = GetDiff(root.get());
    diff_root->Refresh();
}

std::shared_ptr<DiffNode> Differentiator::GetDiff(DiffNode *from) {
    std::shared_ptr<DiffNode> to;
    if (from->func == &PLUS || from->func == &MINUS) {
        if (from->left->func == &CONST) {
            to = from->func == &PLUS ? GetDiff(from->right.get()) : std::make_shared<DiffNode>(&UNARY_MINUS,
                                                                                               GetDiff(from->right.get()),
                                                                                               nullptr);
        } else if (from->right->func == &CONST) {
            to = GetDiff(from->left.get());
        } else {
            to = std::make_shared<DiffNode>(from->func, GetDiff(from->left.get()), GetDiff(from->right.get()));
        }
    } else if (from->func == &MULT) {
        if (from->left->func == &CONST) {
            to = std::make_shared<DiffNode>(from->func, std::make_shared<DiffNode>(*from->left),
                                            GetDiff(from->right.get()));
        } else if (from->right->func == &CONST) {
            to = std::make_shared<DiffNode>(from->func, GetDiff(from->left.get()), std::make_shared<DiffNode>(*from->right));
        } else {
            to = ComplexDiff(from, &PLUS);
        }
    } else if (from->func == &FRAC) {
        if (from->right->func == &CONST) {
            to = std::make_shared<DiffNode>(from->func, GetDiff(from->left.get()), std::make_shared<DiffNode>(*from->right));
        } else {
            to = std::make_shared<DiffNode>(from->func, ComplexDiff(from, &MINUS),
                                            std::make_shared<DiffNode>(&SQR, std::make_shared<DiffNode>(*from->right), nullptr));
        }
    } else if (from->func == &X) {
        to = std::make_shared<DiffNode>(&CONST, 1);
    } else if (from->func == &CONST) {
        to = std::make_shared<DiffNode>(&CONST, 0);
    } else if (from->func == &SIN) {
        to = std::make_shared<DiffNode>(&MULT,
                                        std::make_shared<DiffNode>(&COS,
                                                                   std::make_shared<DiffNode>(*from->left),
                                                                   nullptr),
                                        GetDiff(from->left.get()));
    } else if (from->func == &COS) {
        to = std::make_shared<DiffNode>(&UNARY_MINUS,
                                        std::make_shared<DiffNode>(&MULT, std::make_shared<DiffNode>(&SIN, std::make_shared<DiffNode>(*from->left), nullptr),
                                                                   GetDiff(from->left.get())), nullptr);
    } else if (from->func == &EXP) {
        to = std::make_shared<DiffNode>(&MULT,
                                        std::make_shared<DiffNode>(from->func, std::make_shared<DiffNode>(*from->left), nullptr),
                                        GetDiff(from->left.get()));
    } else if (from->func == &UNARY_MINUS) {
        to = std::make_shared<DiffNode>(&UNARY_MINUS, GetDiff(from->left.get()), nullptr);
    } else if (from->func == &SQR) {
        to = std::make_shared<DiffNode>(&MULT, std::make_shared<DiffNode>(&CONST, 2), GetDiff(from->left.get()));
    }
    to->Refresh();
    return to;
}

std::shared_ptr<DiffNode> Differentiator::ComplexDiff(DiffNode *from, DiffFunc *op) {
    std::shared_ptr<DiffNode> to = std::make_shared<DiffNode>(op,
                                                              std::make_shared<DiffNode>(&MULT,
                                                                                         GetDiff(from->left.get()),
                                                                                         std::make_shared<DiffNode>(*from->right)),
                                                              std::make_shared<DiffNode>(&MULT,
                                                                                         GetDiff(from->right.get()),
                                                                                         std::make_shared<DiffNode>(*from->left)));
    return to;
}

void Differentiator::DiffToString(const std::string &file_name) {
    std::stringstream s;
    s << kTexBegin;
    root->ToString(s);
    s << kTexMiddle;
    diff_root->ToString(s);
    s << kTexEnd;

    std::ofstream out_file;
    std::string out_dir = "../latex/" + file_name;
    std::string rm_command = "rm -rf " + out_dir;
    system(rm_command.c_str());

    std::string mkdir_command = "mkdir " + out_dir;
    system(mkdir_command.c_str());

    std::string tex_file = out_dir + "/" + file_name + ".tex";
    out_file.open(tex_file);
    out_file << s.str();
    out_file.close();

    std::string tex_command = "pdflatex --output-directory=" + out_dir + " " + tex_file;
    const char* command = tex_command.c_str();
    system(command);
}

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
