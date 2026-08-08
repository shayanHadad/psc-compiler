#include "Utils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

string read_file_to_string(const string &path)
{
    ifstream file(path, ios::in);
    if (!file)
    {
        throw runtime_error("Cannot open file: " + path);
    }

    ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

vector<string> split_lines(const string &code)
{
    vector<string> lines;
    string line;
    stringstream ss(code);
    while (getline(ss, line))
        lines.push_back(line);
    return lines;
}

void print_tokens_table(const vector<Token> &tokens,
                        const vector<string> &source_lines)
{
    cout << left
         << setw(6) << "#"
         << setw(15) << "TOKEN"
         << setw(15) << "TYPE"
         << setw(8) << "LINE"
         << setw(8) << "COL-S"
         << setw(8) << "COL-E"
         << " | SOURCE CODE"
         << endl;

    cout << string(90, '-') << endl;

    int index = 1;
    int current_line = -1;

    for (const auto &t : tokens)
    {
        if (current_line != t.line)
        {
            if (current_line != -1)
                cout << string(90, '-') << endl;

            current_line = t.line;

            cout << left
                 << setw(6) << index++
                 << setw(15) << t.name
                 << setw(15) << t.type
                 << setw(8) << t.line
                 << setw(8) << t.column_start
                 << setw(8) << t.column_end
                 << " | "
                 << source_lines[t.line - 1]
                 << endl;
        }
        else
        {
            cout << left
                 << setw(6) << index++
                 << setw(15) << t.name
                 << setw(15) << t.type
                 << setw(8) << t.line
                 << setw(8) << t.column_start
                 << setw(8) << t.column_end
                 << endl;
        }
    }

    cout << string(90, '-') << endl;
}
