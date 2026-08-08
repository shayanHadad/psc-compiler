# PSC Compiler

A small lexer and recursive-descent parser for **PSC**, a tiny Pascal-like
language, built as a course project for a Compilers class.

This project performs **lexical analysis** (tokenizing) and **syntax
analysis** (parsing against a grammar). It does not perform semantic
analysis (no type checking, no scope/declaration checking) or code
generation — its output is a token table and a syntax-validity report.

## Features

- Hand-written tokenizer (regex-based) that reports lexical errors with
  line/column information instead of stopping at the first invalid
  character.
- Recursive-descent parser that validates a PSC program against the
  grammar below and reports every syntax error it finds, with
  line/column position.
- Small, dependency-free C++17 codebase split into logical modules.

## Language: PSC

PSC programs declare `integer` variables, assign arithmetic expressions to
them, and print values with `show`.

```pascal
program abcd;
var
a, b, c: integer;
d: integer;
begin
a = 10;
b = a + 5 * 2;
c = (b - 3) * (a + 1);
d = c / 2 + (a - b);
show(a);
show(b);
show(c);
show(d);
end
```

**Note:** identifiers must start with a letter `a`–`e` and may only
contain `a`–`e` and digits afterwards (e.g. `a`, `b2`, `ce1`). This is an
intentional restriction of the toy language, not a bug.

### Grammar (EBNF)

```
prog       -> "program" id ";" "var" dec_list "begin" stat_list "end"

dec_list   -> dec ":" type ";" { dec ":" type ";" }
dec        -> id { "," id }
type       -> "integer"

stat_list  -> { stat }
stat       -> write | assign
write      -> "show" "(" id ")" ";"
assign     -> id "=" expr ";"

expr       -> term { ("+" | "-") term }
term       -> factor { ("*" | "/") factor }
factor     -> id | number | "(" expr ")"

id         -> a letter in [a-e], followed by [a-e0-9]*
number     -> ["+" | "-"] digit+
```

## Project structure

```
compiler_project/
├── include/            # Header files (class declarations)
│   ├── Token.hpp
│   ├── Tokenizer.hpp
│   ├── Parser.hpp
│   └── Utils.hpp
├── src/                 # Implementation files
│   ├── Tokenizer.cpp
│   ├── Parser.cpp
│   ├── Utils.cpp
│   └── main.cpp
└── examples/            # Sample .psc programs
    ├── program1.psc
    ├── program2.psc
    └── fail1.psc         # deliberately invalid, to demonstrate error reporting
```

## Build

Requires a C++17 compiler (e.g. g++ 9+).

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/Tokenizer.cpp src/Parser.cpp src/Utils.cpp -o compiler
```

## Usage

```bash
# Linux / macOS
./compiler examples/program1.psc

# Windows (cmd)
compiler.exe examples\program1.psc

# Windows (PowerShell)
.\compiler.exe examples\program1.psc
```

The program prints a token table followed by the syntax analysis result.
Try `examples/fail1.psc` to see how lexical/syntax errors are reported.

## Authors

- Parsa Basiri
- Shayan Hadad

_(the project name **PSC** comes from **P**arsa + **S**hayan + **C**ompiler)_

## License

This project is available under the MIT License — see [LICENSE](LICENSE)
for details.
