#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string input_file, output_file;

    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }

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

    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << "Quark is still under development." << std::endl;

    return 0;
}