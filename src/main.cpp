#include <iostream>

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            cerr << "Usage: " << argv[0] << " <input.psc>\n";
            return 1;
        }

        string file_path = argv[1];
        string code = read_file_to_string(file_path);
        vector<string> source_lines = split_lines(code);

        cout << "=== Tokenization ===" << endl;
        Tokenizer tokenizer(code);
        auto [tokens, errors] = tokenizer.scan();

        if (!errors.empty())
        {
            cout << "\n=== Errors during tokenization ===" << endl;
            for (const auto &error : errors)
            {
                cout << error << endl;
            }
        }
        else
        {
            print_tokens_table(tokens, source_lines);
            cout << "\n=== Syntax Analysis ===" << endl;
            Parser parser(tokens);
            parser.parse();
        }
    }
    catch (const exception &e)
    {
        cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
