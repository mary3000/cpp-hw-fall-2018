#include <iostream>
#include "src/Processor.h"

//Example: type ./Processor ./data/sum_cin.txt

void TestProcessor(std::istream *in, const std::string &file_name) {
    std::ifstream file(file_name);
    Processor p(file, 1000);
    file.close();
    if (in == &std::cin) {
        std::cout << std::endl;
    }
    p.Run(in, std::cout);
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    TestProcessor(&std::cin, argv[1]);
    return 0;
}