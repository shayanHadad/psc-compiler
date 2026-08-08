#include "Parser.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

void Parser::parse()
{
    try
    {
        parse_prog();
        if (errors.empty())
        {
            cout << "Syntax analysis successful." << endl;
        }
        else
        {
            cout << "Syntax analysis failed with the following errors:" << endl;
            for (const auto &error : errors)
            {
                cout << error << endl;
            }
        }
    }
    catch (const exception &e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}

void Parser::add_error(const string &message)
{
    if (current_token_index < tokens.size())
    {
        const Token &token = tokens[current_token_index];
        errors.push_back(message + " at line " + to_string(token.line) +
                         ", column " + to_string(token.column_start) +
                         "-" + to_string(token.column_end));
    }
    else
    {
        errors.push_back(message + " at end of input");
    }
}

bool Parser::parse_prog()
{
    if (!match_token("keyword", {"program"}))
    {
        add_error("Expected 'program'");
        return false;
    }
    if (!parse_id())
    {
        add_error("Expected an identifier after 'program'");
    }
    if (!match_token("delimiter", {";"}))
    {
        add_error("Expected ';' after program declaration");
    }
    if (!match_token("keyword", {"var"}))
    {
        add_error("Expected 'var' section");
    }
    parse_dec_list();
    if (!match_token("keyword", {"begin"}))
    {
        add_error("Expected 'begin' to start the program body");
    }
    parse_stat_list();
    if (!match_token("keyword", {"end"}))
    {
        add_error("Expected 'end' to close the program");
    }
    return true;
}

bool Parser::parse_id()
{
    return match_token("identifier", {});
}

void Parser::parse_dec_list()
{
    if (!parse_dec())
    {
        add_error("Expected a declaration");
    }
    if (!match_token("delimiter", {":"}))
    {
        add_error("Expected ':' after declaration");
    }
    if (!parse_type())
    {
        add_error("Expected a type specification");
    }
    if (!match_token("delimiter", {";"}))
    {
        add_error("Expected ';' after type specification");
    }
    while (lookahead("identifier", {}))
    {
        parse_dec_list();
    }
}

bool Parser::parse_dec()
{
    if (!parse_id())
    {
        return false;
    }
    while (lookahead("delimiter", {","}))
    {
        if (!match_token("delimiter", {","}))
        {
            add_error("Expected ',' between declarations");
        }
        if (!parse_id())
        {
            add_error("Expected identifier after ',' in declarations");
        }
    }
    return true;
}

bool Parser::parse_type()
{
    return match_token("keyword", {"integer"});
}

void Parser::parse_stat_list()
{
    while (!lookahead("keyword", {"end"}) && current_token_index < tokens.size())
    {
        if (!parse_stat())
        {
            add_error("Invalid statement");
        }
    }
}

bool Parser::parse_stat()
{
    if (lookahead("keyword", {"show"}))
    {
        return parse_write();
    }
    else if (lookahead("identifier", {}))
    {
        return parse_assign();
    }
    else
    {
        add_error("Unexpected statement");
        return false;
    }
}

bool Parser::parse_write()
{
    if (!match_token("keyword", {"show"}))
    {
        return false;
    }
    if (!match_token("delimiter", {"("}))
    {
        add_error("Expected '(' after 'show'");
    }
    if (!parse_id())
    {
        add_error("Expected identifier in 'show' statement");
    }
    if (!match_token("delimiter", {")"}))
    {
        add_error("Expected ')' after identifier in 'show' statement");
    }
    if (!match_token("delimiter", {";"}))
    {
        add_error("Expected ';' after 'show' statement");
    }
    return true;
}

bool Parser::parse_assign()
{
    if (!parse_id())
    {
        return false;
    }
    if (!match_token("operator", {"="}))
    {
        add_error("Expected '=' in assignment");
    }
    if (!parse_expr())
    {
        add_error("Expected expression in assignment");
    }
    if (!match_token("delimiter", {";"}))
    {
        add_error("Expected ';' after assignment");
    }
    return true;
}

bool Parser::parse_expr()
{
    if (!parse_term())
    {
        return false;
    }
    while (lookahead("operator", {"+", "-"}))
    {
        match_token("operator", {"+", "-"});
        if (!parse_term())
        {
            add_error("Expected term after operator in expression");
        }
    }
    return true;
}

bool Parser::parse_term()
{
    if (!parse_factor())
    {
        return false;
    }
    while (lookahead("operator", {"*", "/"}))
    {
        match_token("operator", {"*", "/"});
        if (!parse_factor())
        {
            add_error("Expected factor after operator in term");
        }
    }
    return true;
}

bool Parser::parse_factor()
{
    if (lookahead("identifier", {}))
    {
        return parse_id();
    }
    else if (lookahead("number", {}))
    {
        return parse_number();
    }
    else if (lookahead("delimiter", {"("}))
    {
        if (!match_token("delimiter", {"("}))
        {
            add_error("Expected '('");
        }
        if (!parse_expr())
        {
            add_error("Expected expression inside parentheses");
        }
        if (!match_token("delimiter", {")"}))
        {
            add_error("Expected ')' after expression");
        }
        return true;
    }
    else
    {
        add_error("Unexpected factor");
        return false;
    }
}

bool Parser::parse_number()
{
    if (lookahead("operator", {"+", "-"}))
    {
        match_token("operator", {"+", "-"});
    }
    if (!match_token("number", {}))
    {
        add_error("Expected a number");
        return false;
    }
    return true;
}

bool Parser::match_token(const string &token_type, const vector<string> &values)
{
    if (current_token_index >= tokens.size())
    {
        return false;
    }
    const Token &token = tokens[current_token_index];

    if (token.type == token_type)
    {
        if (values.empty())
        {
            current_token_index++;
            return true;
        }
        else
        {
            for (const auto &value : values)
            {
                if (token.name == value)
                {
                    current_token_index++;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Parser::lookahead(const string &token_type, const vector<string> &values)
{
    if (current_token_index >= tokens.size())
    {
        return false;
    }
    const Token &token = tokens[current_token_index];

    if (token.type == token_type)
    {
        if (values.empty())
        {
            return true;
        }
        else
        {
            for (const auto &value : values)
            {
                if (token.name == value)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
