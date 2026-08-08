#pragma once

#include <string>
#include <vector>

#include "Token.hpp"

// Recursive-descent parser that checks a token stream against the
// PSC grammar. It does not build an AST and performs no semantic
// analysis (no type checking, no scope/declaration checking) -
// it only reports whether the input is syntactically valid.
class Parser
{
public:
    explicit Parser(const std::vector<Token> &tokens)
        : tokens(tokens), current_token_index(0)
    {
    }

    // Runs the parse and prints the result (success or the list of
    // syntax errors found) to stdout.
    void parse();

private:
    std::vector<Token> tokens;
    size_t current_token_index;
    std::vector<std::string> errors;

    void add_error(const std::string &message);

    // Grammar rules (each roughly corresponds to one non-terminal):
    //   prog       -> "program" id ";" "var" dec_list "begin" stat_list "end"
    bool parse_prog();
    bool parse_id();
    //   dec_list   -> dec ":" type ";" { dec_list }
    void parse_dec_list();
    //   dec        -> id { "," id }
    bool parse_dec();
    //   type       -> "integer"
    bool parse_type();
    //   stat_list  -> { stat }
    void parse_stat_list();
    //   stat       -> write | assign
    bool parse_stat();
    //   write      -> "show" "(" id ")" ";"
    bool parse_write();
    //   assign     -> id "=" expr ";"
    bool parse_assign();
    //   expr       -> term { ("+"|"-") term }
    bool parse_expr();
    //   term       -> factor { ("*"|"/") factor }
    bool parse_term();
    //   factor     -> id | number | "(" expr ")"
    bool parse_factor();
    bool parse_number();

    bool match_token(const std::string &token_type, const std::vector<std::string> &values);
    bool lookahead(const std::string &token_type, const std::vector<std::string> &values);
};
