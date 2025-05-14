#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "lexer.hpp"
#include "constants.hpp"
#include "utils.hpp"

auto main(int argc, char* argv[]) -> int {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << '\n';
        return 1;
    }

    std::string input_file;
    std::string output_file;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "Error: -o option requires an argument." << '\n';
                return 1;
            }
        } else {
            if (input_file.empty()) {
                input_file = arg;
            } else {
                std::cerr << "Error: Too many input files specified." << '\n';
                return 1;
            }
        }
    }

    if (input_file.empty()) {
        std::cerr << "Error: No input file specified." << '\n';
        return 1;
    }

    if (output_file.empty()) {
        std::cerr << "Error: No output file specified. Use -o <filename>." << '\n';
        return 1;
    }

    // Read input file
    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
        std::cerr << "Error: Failed to open input file: " << input_file << '\n';
        return 1;
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    // Compilation logic
    auto *logger = QuarkLogger::get_instance();
    logger->info("Quark compilation has started...");
    Lexer lexer(buffer.str());
    lexer.get_next_token();
    logger->info("Quark lexing completed...");
    logger->info("Quark compilation done...");

    return 0;
}
