#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.hpp"

auto main(int argc, char* argv[]) -> int {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }

    std::string input_file;
    std::string output_file;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "Error: -o option requires an argument." << std::endl;
                return 1;
            }
        } else {
            if (input_file.empty()) {
                input_file = arg;
            } else {
                std::cerr << "Error: Too many input files specified." << std::endl;
                return 1;
            }
        }
    }

    if (input_file.empty()) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }

    if (output_file.empty()) {
        std::cerr << "Error: No output file specified. Use -o <filename>." << std::endl;
        return 1;
    }

    // Read input file
    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
        std::cerr << "Error: Failed to open input file: " << input_file << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    std::string source_code = buffer.str();

    Lexer lexer(source_code);
    Token token;
    while ((token = lexer.get_next_token()).type != TokenType::END_OF_FILE) {
        std::cout << token_to_string(token.type) << " " << token.value << std::endl;
    }

    return 0;
}