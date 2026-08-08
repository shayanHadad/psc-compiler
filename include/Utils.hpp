#pragma once

#include <string>
#include <vector>

#include "Token.hpp"

// Reads the whole content of a file into a string. Throws
// std::runtime_error if the file cannot be opened.
std::string read_file_to_string(const std::string &path);

// Splits source code into individual lines (without the newline
// characters), used for displaying source context in the token table.
std::vector<std::string> split_lines(const std::string &code);

// Prints a formatted table of tokens next to their source line,
// grouping consecutive tokens that belong to the same line.
void print_tokens_table(const std::vector<Token> &tokens,
                         const std::vector<std::string> &source_lines);
