#pragma once

#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "Token.hpp"

// Converts PSC source code into a stream of Tokens.
// Any characters that cannot be matched to a known token pattern
// are reported back as lexical errors instead of throwing.
class Tokenizer
{
public:
    explicit Tokenizer(const std::string &code) : code(code) {}

    // Scans the whole source and returns (tokens, errors).
    std::pair<std::vector<Token>, std::vector<std::string>> scan();

private:
    std::string code;
    std::vector<Token> tokens;
    std::vector<std::string> errors;

    const std::unordered_set<std::string> KEYWORDS = {
        "program", "var", "begin", "end", "integer", "show"};

    const std::unordered_set<char> OPERATORS = {'+', '-', '*', '/', '='};
    const std::unordered_set<char> DELIMITERS = {'(', ')', ';', ',', ':'};
    const std::unordered_set<char> WHITESPACE = {' ', '\t', '\n'};

    std::unordered_map<std::string, std::regex> token_patterns = {
        {"identifier", std::regex("\\b[a-e][a-e0-9]*\\b")},
        {"keyword", std::regex("\\b(program|var|begin|end|integer|show)\\b")},
        {"delimiter", std::regex("(\\(|\\)|;|,|:)")},
        {"number", std::regex("\\b[+-]?\\d+\\b")},
        {"operator", std::regex("(\\+|\\-|\\*|\\/|=)")}};
};
