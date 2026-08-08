#include "Tokenizer.hpp"

#include <sstream>

using namespace std;

pair<vector<Token>, vector<string>> Tokenizer::scan()
{
    string line;
    stringstream ss(code);
    int line_number = 0;

    while (getline(ss, line, '\n'))
    {
        line_number++;
        int col_number = 1;
        int line_length = static_cast<int>(line.length());

        while (col_number <= line_length)
        {
            char current_char = line[col_number - 1];

            if (WHITESPACE.find(current_char) != WHITESPACE.end())
            {
                col_number++;
                continue;
            }

            bool matched = false;
            smatch match;
            string token_type;
            string token_value;
            string substr = line.substr(col_number - 1);

            // Try every pattern and keep the LONGEST match that starts
            // right here (maximal munch). Trying patterns in an
            // unspecified order and stopping at the first hit (as this
            // used to do) could pick a shorter token depending on
            // iteration order - e.g. matching just part of a keyword.
            for (const auto &pattern_pair : token_patterns)
            {
                if (regex_search(substr, match, pattern_pair.second) &&
                    match.position() == 0)
                {
                    if (!matched || match.length() > static_cast<long>(token_value.length()))
                    {
                        token_value = match.str();
                        token_type = pattern_pair.first;
                        matched = true;
                    }
                }
            }

            if (matched)
            {
                int col_start = col_number;
                int col_end = col_number + static_cast<int>(token_value.length()) - 1;

                tokens.emplace_back(token_value, token_type, line_number, col_start, col_end);
                col_number += static_cast<int>(token_value.length());
            }
            else
            {
                errors.push_back("Error: Invalid token at line " +
                                 to_string(line_number) +
                                 ", column " + to_string(col_number));
                col_number++;
            }
        }
    }

    return {tokens, errors};
}
