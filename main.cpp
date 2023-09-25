#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
using namespace std;

int main() {
    ifstream code("quark.txt");
    string input;
    ostringstream ss;
    ss << code.rdbuf();
    input = ss.str();
    auto bruh = LexInput(input);
    DebugPrintTokenArray(bruh);
    cout << eval(bruh);
}
