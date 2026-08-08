#pragma once

#include <string>
#include <ostream>

// Represents a single lexical token produced by the Tokenizer.
class Token
{
public:
    std::string name; // the raw text of the token, e.g. "begin", "+", "42"
    std::string type;  // the token category, e.g. "keyword", "operator"
    int line;
    int column_start;
    int column_end;

    Token(const std::string &name, const std::string &type,
          int line, int column_start, int column_end)
        : name(name), type(type), line(line),
          column_start(column_start), column_end(column_end) {}

    friend std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        os << "Token(" << token.name << ", " << token.type
           << ", line=" << token.line
           << ", col=" << token.column_start << "-" << token.column_end << ")";
        return os;
    }
};
